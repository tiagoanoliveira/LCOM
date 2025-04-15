# Lab 3 - i8042, the PC Keyboard and its Controller

## 1. PC Keyboard

O teclado é um dos dispositivos de entrada mais fundamentais de um computador. Para compreender o funcionamento interno de um teclado, é essencial entender como os sinais físicos sáo convertidos em informação digital interpretável pelo computador.

### _Scancodes:_ a linguagem do teclado

Quando um utilizador interage com o teclado, o hardware não transmite diretamente o código ASCII ou Unicode correspondente à tecla pressionada. Em vez disso, gera um **scancode** — um código numérico que representa a posição física da tecla no teclado, independente do idioma, layout ou fabricante.

Esta abordagem permite aos fabricantes de teclados uma grande flexibilidade, uma vez que o mesmo _hardware_ pode ser utilizado em diferentes países com diferentes configurações de teclado - ou seja, basta mudar os autocolantes do teclado e o idioma configurado no sistema operativo e voilá!

O sistema operativo utiliza tabelas de mapeamento **(keymaps)** para converter os scancodes recebidos no texto correspondente ao idioma configurado. É por isso que podemos mudar o layout do teclado via software sem alterar o hardware.

Estes scancodes são então processados pelo controlador do teclado (Keyboard Controller), responsável por gerir a comunicação com o processador e gerar interrupções ou disponibilizar os dados para leitura, conforme o método usado — interrupções, polling ou com timeout.

Existem dois tipos principais de scancodes:

- `makecode`: gerado quando uma tecla é pressionada;
- `breakcode`: gerado quando uma tecla é largada.

Geralmente, o `breakcode` de uma tecla difere do `makecode` pelo bit mais significativo - o MSB de um `breakcode` está ativo, enquanto o MSB de um `makecode` não está. Por exemplo, o `makecode` da tecla ESC é 0x01 (00000001), enquanto o seu `breakcode` é 0x81 (10000001).

Uma forma fácil de saber se um scancode era ou não um breakcode seria:
~~~C
bool is_breakcode(uint8_t scancode) {
   /* Verifica se o bit mais significativo (bit 7) está ativo
   Se estiver ativo, é um breakcode
   Se não estiver ativo, é um makecode */
   if (scancode & BIT(7)){
      returne true;
   }
   else {
      return false;
   }
}
~~~
Para algumas teclas especiais (como teclas de função ou teclas de controlo adicionais), os scancodes podem ter dois bytes. Scancodes de dois bytes geralmente utilizam 0xE0 como primeiro byte, tanto no `makecode` quanto no `breakcode`.

## 2. O Controlador do Teclado (KBC - i8042)

Nos computadores modernos, a comunicação entre o teclado e o processador é mediada por um componente eletrónico que fornece a funcionalidade do i8042, conhecido como **Keyboard Controller (KBC)**. Este controlador não gere apenas a comunicação com o teclado, mas também pode controlar o rato de dois botões em muitos sistemas.

O KBC utiliza um sistema de portas de entrada/saída e registos que permitem a comunicação bidirecional:

- **Porta do Status Register (0x64):** Permite ler o estado atual do controlador;
- **Porta de Comandos (0x64):** Usada para enviar comandos ao controlador;
- **Porta de Dados (0x60):** Usada para ler dados do controlador ou enviar dados para o controlador.

O registo de estado (status register) é formado por 8 bits e fornece informações importantes, como:

~~~lua
+-------+---------+---------------------------+
|  Bit  | Função quando ativo (1)             |
+-------+---------+---------------------------+
|   7   | Erro de paridade detetado           |
+-------+---------+---------------------------+
|   6   | Ocorreu timeout                     |
+-------+---------+---------------------------+
|   5   | Mouse data                          |
+-------+---------+---------------------------+
|   4   | Valor específico da implementação   |
+-------+---------+---------------------------+
|   3   | Comando / Dado (1=comando; 0=dado)  |
+-------+---------+---------------------------+
|   2   | Estado do sistema                   |
+-------+---------+---------------------------+
|   1   | Buffer de entrada está cheio        |
+-------+---------+---------------------------+
|   0   | Buffer de saída está cheio          |
+-------+---------+---------------------------+
~~~
Exemplo de leitura do status e análise do estado do _input buffer_:
~~~C
uint8_t read_KBC_status(uint8_t *status) {
    return util_sys_inb(0x64, status);
}

void check_input_buffer() {
    uint8_t status;
    read_KBC_status(&status);
    
    if (status & BIT(1)) {
        printf("Input buffer is full, can't write now\n");
    } else {
        printf("Input buffer is available for writing\n");
    }
}
~~~

### Interações com o KBC

Quando interagimos com o KBC, devem ser feitas duas considerações importantes:

- O **buffer de entrada é finito e pode estar cheio**, impedindo a inserção de novos comandos;
- O **KBC é relativamente lento** (ordem de milissegundos), obrigando a realizar várias tentativas para uma só operação.

Para lidar com essas limitações, é necessária uma estratégia de comunicação robusta, geralmente implementada com tentativas múltiplas e tempos de espera. Geralmente 10 tentativas e 20 milissegundos entre cada uma é suficiente (vê com mais detalhe o ponto 4.2 de "Minix 3 Notes - Lab 3")

Um exemplo de escrita de comando para o KBC com múltiplas tentativas poderia ser:
~~~C
int write_KBC_command(uint8_t port, uint8_t commandByte) {
    uint8_t status;
    uint8_t attempts = 10;  // Número máximo de tentativas

    while (attempts > 0) {
        // Verificar se o buffer de entrada está disponível
        if (read_KBC_status(&status) != 0) {
            printf("Error: Failed to read status\n");
            return 1;
        }

        // Se o buffer de entrada não estiver cheio, podemos escrever
        if ((status & BIT(1)) == 0) {
            if (sys_outb(port, commandByte) != 0) {
                printf("Error: Failed to write command\n");
                return 1;
            }
            return 0;  // Sucesso: comando inserido
        }
        
        // Esperar 20ms antes de tentar novamente
        tickdelay(micros_to_ticks(20000));
        attempts--;
    }
    
    printf("Error: Maximum attempts exceeded\n");
    return 1;  // Falha após esgotar as tentativas
}
~~~

Da mesma forma, a leitura do buffer de saída requer verificação do status e tratamento de erros, com uma estrutura muito semelhante à do exemplo anterior.

### Modos de Operação

O KBC pode operar em dois modos principais:
- **modo de interrupções;**
- **modo de polling.**

<p align="center">
  <img src="../resources/images/Polling_Interrupts_versão1.png" alt="Polling vs. Interrupts" width="50%">
  <p align="center">Filha pergunta ao pai se já chegaram a todo o minuto. No exemplo de Interrupts, o pai diz à filha quando chegam e só aí é que ela acorda. Elaborado com IA.</p>

#### Modo de Interrupções

Neste modo, sempre que uma tecla é pressionada, o KBC gera uma interrupção (IRQ linha 1). O processador interrompe a sua execução normal para processar essa interrupção através de um manipulador de interrupções (interrupt handler).

As interrupções oferecem um método eficiente para lidar com entradas do teclado, pois o processador só é interrompido quando realmente há dados a serem processados.

~~~C
~~~

#### Modo de Polling

No modo de polling, o software verifica continuamente o estado do KBC para determinar se há novos dados a serem lidos. Embora menos eficiente em termos de uso do processador, este método é mais simples de implementar e útil em certas situações.

É importante notar que, ao usar polling em sistemas como o Minix 3, é essencial restaurar o modo de interrupções antes de encerrar o programa, caso contrário, o teclado pode parar de responder.




## 2. Funções de teste esperadas

Neste laboratório é esperado que implementes as seguintes funções de teste principais:
- **kbd_test_scan** - Leitura de scancodes usando um handler de interrupções em C;
- **kbd_test_poll** - Leitura de scancodes via polling (sem interrupções);
- **kbd_test_timed_scan** - Combinar os handlers de interrupções do teclado e do timer.

## 3. Ficheiros necessários

Neste lab irás precisar de criar os seguintes ficheiros:
- **i8042.h:** Para definir constantes relacionadas com o controlador do teclado (KBC);
- **keyboard.h:** Para declarar as funções relacionadas com o teclado;
- **keyboard.c:** Para implementar as funções relacionadas com o teclado.

Os ficheiros i8254.h, timer.c e utils.c já existem no Lab2, portanto podes fazer CTRL+C - CTRL+V para a pasta deste lab.

## 4. Definir as constantes no i8042.h

No ponto 3.2. da documentação de apoio para o [Lab3](https://pages.up.pt/~up722898/aulas/lcom2425/lab3/lab3_03.html) é fornecido uma descrição funcional do i8042, incluindo com detalhe todos os comandos possíveis do mesmo. No entanto, como é expectável, não precisamos de todos, pois apenas precisas de definir constantes para:
- Endereços das portas de I/O do KBC;
- Códigos de comando para o KBC;
- Bits de estado do KBC;
- O IRQ do teclado.

## 5. Implementar funções básicas do teclado

No ficheiro keyboard.c








> Quais os conhecimentos-base que devo ter para este lab?

Em príncipio, se dominaste todos os conceitos descritos no [Lab2](https://github.com/tiagoleic02/LCOM/blob/master/lab2/README.md) não terás grandes dificuldades em realizar este lab, principalmente se tiveres mantido o teu código limpo e organizado.
Para este lab precisas:
1. [x] Compreender como funciona a comunicação entre um programa/driver e o controlador do dispositivo I/O, incluindo:
      1. [x] Utilização de funções _sys_inb_ e _sys_outb_;
      2. [x] Necessidade de usar uma função auxiliar que converta os 32 bits do apontador usado em _sys_inb_ em 8 bits;
2. [x] Usar um registo de controlo para informar o controlador do que pretendemos acessar ou modificar antes;