# Lab 3 - i8042, the PC Keyboard and its Controller

## 1. PC Keyboard

O teclado é um dos dispositivos de entrada mais fundamentais de um computador. No entanto, o seu funcionamento interno é mais complexo do que aparenta. Quando alguém pressiona ou larga uma tecla, o teclado não envia diretamente o código ASCII ou outra codificação comum do carácter correspondente, mas sim um código específico chamado **scancode.**

Os scancodes são códigos que dependem apenas da posição física da tecla no teclado, independentemente do idioma ou layout utilizado. Esta abordagem permite aos fabricantes de teclados uma grande flexibilidade, uma vez que o mesmo _hardware_ pode ser utilizado em diferentes países com diferentes configurações de teclado - ou seja, basta mudar os autocolantes do teclado e o idioma configurado no sistema operativo e voilá!

Existem dois tipos principais de scancodes:

- `makecode`: gerado quando uma tecla é pressionada;
- `breakcode`: gerado quando uma tecla é largada.

Geralmente, o `breakcode` de uma tecla difere do `makecode` pelo bit mais significativo - o MSB de um `breakcode` está ativo, enquanto o MSB de um `makecode` não está. Por exemplo, o `makecode` da tecla ESC é 0x01 (00000001), enquanto o seu `breakcode` é 0x81 (10000001).

Uma forma fácil de saber se um scancode era ou não um breakcode seria:
~~~C
bool is_breakcode(uint8_t scancode) {
   // Verifica se o bit mais significativo (bit 7) está ativo
   // Se estiver ativo (1), é um breakcode
   // Se não estiver ativo (0), é um makecode
   return (scancode & BIT(7)) != 0;
}
~~~
A maioria dos scancodes de PC têm um byte de comprimento, embora algumas teclas especiais tenham scancodes mais longos. Scancodes de dois bytes geralmente utilizam 0xE0 como primeiro byte, tanto no `makecode` quanto no `breakcode`.

## 2. O Controlador do Teclado (KBC - i8042)

Nos computadores modernos, a comunicação entre o teclado e o processador é mediada por um componente eletrónico que fornece a funcionalidade do i8042, conhecido como **Keyboard Controller (KBC)**. Este controlador não gere apenas a comunicação com o teclado, mas também pode controlar o rato de dois botões em muitos sistemas.

O KBC funciona com uma arquitetura baseada em portas de entrada/saída e registos:

- **Porta do Status Register (0x64):** Permite ler o estado atual do controlador;
- **Porta de Comandos (0x64):** Usada para enviar comandos ao controlador;
- **Porta de Dados (0x60):** Usada para ler dados do controlador ou enviar dados para o controlador.

O registo de estado (status register) fornece informações importantes, como:

- Se ocorreu um erro de paridade (bit 7);
- Se ocorreu um erro de timeout (bit 6);
- Se o buffer de entrada está cheio (bit 1);
- Se o buffer de saída está cheio (bit 0).

### Interações com o KBC

Quando interagimos com o KBC, devem ser feitas duas considerações importantes:

- O buffer de entrada é finito e pode estar cheio, impedindo a inserção de novos comandos;
- O KBC é relativamente lento (ordem de milissegundos), obrigando a realizar várias tentativas para uma só operação.

Para lidar com essas limitações, é comum implementar um ciclo _while_, onde os comandos são repetidos após pequenos atrasos se não forem bem-sucedidos inicialmente.

### Modos de Operação

O KBC pode operar em dois modos principais:

#### Modo de Interrupções

Neste modo, sempre que uma tecla é pressionada, o KBC gera uma interrupção (IRQ linha 1). O processador interrompe a sua execução normal para processar essa interrupção através de um manipulador de interrupções (interrupt handler).

As interrupções oferecem um método eficiente para lidar com entradas do teclado, pois o processador só é interrompido quando realmente há dados a serem processados.

#### Modo de Polling

No modo de polling, o software verifica continuamente o estado do KBC para determinar se há novos dados a serem lidos. Embora menos eficiente em termos de uso do processador, este método é mais simples de implementar e útil em certas situações.

É importante notar que, ao usar polling em sistemas como o Minix 3, é essencial restaurar o modo de interrupções antes de encerrar o programa, caso contrário, o teclado pode parar de responder.

<p align="center">
  <img src="../resources/images/Polling_Interrupts_versão1.png" alt="Polling vs. Interrupts" width="60%">
  <p align="center">Filha pergunta ao pai se já chegaram a todo o minuto. No exemplo de Interrupts, o pai diz à filha quando chegam e só aí é que ela acorda. Elaborado com IA.</p>

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