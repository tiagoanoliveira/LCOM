# Lab 3 - i8042, the PC Keyboard and its Controller

## 1. Funções de teste esperadas

Neste laboratório é esperado que implementes as seguintes funções de teste principais:
- **kbd_test_scan** - Leitura de scancodes usando um handler de interrupções em C;
- **kbd_test_poll** - Leitura de scancodes via polling (sem interrupções);
- **kbd_test_timed_scan** - Combinar os handlers de interrupções do teclado e do timer.

## 2. Ficheiros necessários

Neste lab irás precisar de criar os seguintes ficheiros:
- **`i8042.h`:** Para definir constantes relacionadas com o controlador do teclado (KBC);
- **`keyboard.c`** (com `keyboard.h` correspondente): Para implementar as funções relacionadas com o teclado:
  - `kbd_subscribe_int` - Subscreve interrupções do teclado em modo exclusivo;
  - `kbd_unsubscribe_int` - Cancela a subscrição de interrupções;
  - `kbc_ih` - Interrupt handler para processar interrupções do teclado;
- **`KBC.c`** (com KBC.h correspondente): Implementa funções de baixo nível para comunicação direta com o i8042:
  - `kbc_read_status` - Lê o registo de status do KBC;
  - `kbc_read_output` - Lê o buffer de saída do KBC, verificando erros de paridade e timeout;
  - `kbc_write_command` - Escreve um comando no KBC, lidando com tentativas e atrasos;
  - `kbc_restore_interrupts` - Restaura a configuração do KBC para permitir interrupções.

Também irás precisar dos ficheiros i8254.h, timer.c e utils.c que já existem no Lab2, portanto podes fazer CTRL+C - CTRL+V para a pasta deste lab.

## 3. Definir as constantes no i8042.h

No ponto 3.2. da documentação de apoio para o [Lab3](https://pages.up.pt/~up722898/aulas/lcom2425/lab3/lab3_03.html) é fornecido uma descrição funcional do i8042, incluindo com detalhe todos os comandos possíveis do mesmo. No entanto, como é expectável, não precisamos de todos, pois apenas precisas de definir constantes para:
- Endereços das portas de I/O do KBC;
- Códigos de comando para o KBC;
- Bits de estado do KBC;
- O IRQ do teclado.

## 4. PC Keyboard

O teclado é um dos dispositivos de entrada mais fundamentais de um computador. Para compreender o funcionamento interno de um teclado, é essencial entender como os sinais físicos sáo convertidos em informação digital interpretável pelo computador.

### _Scancodes:_ a linguagem do teclado

Quando um utilizador interage com o teclado, o hardware não transmite diretamente o código ASCII ou Unicode correspondente à tecla pressionada. Em vez disso, gera um **scancode** — um código numérico que **representa a posição física da tecla no teclado**, independente do idioma, layout ou fabricante.

O sistema operativo utiliza tabelas de mapeamento **(keymaps)** para converter os scancodes recebidos no texto correspondente ao idioma configurado. É por isso que podemos mudar o layout do teclado via software sem alterar o hardware.

Estes scancodes são então processados pelo controlador do teclado **(Keyboard Controller)**, responsável por gerir a comunicação com o processador e gerar interrupções ou disponibilizar os dados para leitura, conforme o método usado — interrupções, polling ou com timeout.

Existem dois tipos principais de scancodes:

- `makecode`: gerado quando uma tecla é pressionada;
- `breakcode`: gerado quando uma tecla é largada.

Geralmente, o `breakcode` de uma tecla difere do `makecode` pelo bit mais significativo - o **MSB de um `breakcode` está ativo**, enquanto o **MSB de um `makecode` não está**. Por exemplo, o `makecode` da tecla ESC é 0x01 (00000001), enquanto o seu `breakcode` é 0x81 (10000001).

Uma forma fácil de saber se um scancode é ou não um `breakcode` pode ser:
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

## 5. O Controlador do Teclado (KBC - i8042)

Nos computadores modernos, a comunicação entre o teclado e o processador é mediada por um componente eletrónico que fornece a funcionalidade do i8042, conhecido como **Keyboard Controller (KBC)**. Este controlador não gere apenas a comunicação com o teclado, mas também pode controlar o rato de dois botões em muitos sistemas.

### Registos e portas de input/output;

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

Os bits que nos vão interessar neste lab serão apenas o 0, 1, 6 e 7.

### Leitura do status do KBC

A leitura do status é uma operação fundamental para qualquer interação com o KBC. Podes fazê-la da seguinte forma:
~~~C
uint8_t status;
util_sys_inb(0x64, &status);

// Verificação de erros
if (status & BIT(7)) {
    printf("Erro de paridade detectado\n");
}
if (status & BIT(6)) {
    printf("Erro de timeout detectado\n");
}

// Verificação de buffers
if (status & BIT(1)) {
    printf("Buffer de entrada cheio - não é possível enviar comandos\n");
}
if (status & BIT(0)) {
    printf("Buffer de saída cheio - dados disponíveis para leitura\n");
}
~~~

### Interações com o KBC

A comunicação com o KBC apresenta dois desafios significativos:

- **Buffer limitado:** O buffer de entrada é finito e pode estar cheio, impedindo a inserção de novos comandos;
- **Resposta lenta:** O KBC é relativamente lento, com tempos de resposta na ordem dos milissegundos, obrigando a realizar várias tentativas para uma só operação.

Para lidar com essas limitações, é necessária uma estratégia de comunicação robusta, geralmente implementada com **tentativas múltiplas e tempos de espera**. Geralmente **10 tentativas e 20 milissegundos entre cada uma** é suficiente (a especificação IBM do i8042 exige que o teclado responda com um comando em 20ms).

Estes desafios exigem uma abordagem robusta para comunicação, implementada através de tentativas múltiplas com intervalos de espera entre elas.

### Leitura e escrita de comandos no KBC

Para escrever comandos no KBC, primeiro verificamos se o buffer de entrada está disponível (bit 1 do status não está ativo). Como o KBC pode estar ocupado, implementamos um sistema de tentativas com esperas:

~~~C
int write_KBC_command(uint8_t port, uint8_t commandByte) {
    uint8_t status;
    uint8_t attempts = 10;  // Número máximo de tentativas - normalmente suficiente

    while (attempts > 0) {
        // Ler o status atual do KBC
        if (read_KBC_status(&status) != 0) {
            printf("Error: Status not available!\n");
            return 1;
        }

        // Verificar se o buffer de entrada não está cheio
        if ((status & BIT(1)) == 0) {
            // Buffer disponível, podemos escrever o comando
            if (sys_outb(port, commandByte) != 0) {
                printf("Error: Could not write commandByte!\n");
                return 1;
            }
            return 0;  // Sucesso: comando inserido no i8042
        }
        
        // Buffer cheio, esperar 20ms antes de tentar novamente
        // Esta espera é essencial para dar tempo ao KBC de processar comandos anteriores
        tickdelay(micros_to_ticks(20000));
        attempts--;
    }
    
    // Se chegamos aqui, todas as tentativas falharam
    return 1;
}
~~~
A função `tickdelay()` é usada para suspender a execução por um intervalo especificado, dando ao KBC tempo para processar operações pendentes. A função `micros_to_ticks()` converte microssegundos em ticks do processador, garantindo o intervalo correto entre tentativas.

### Leitura de dados do KBC

Da mesma forma, a leitura de dados do KBC requer verificação do status. Os dados só são fiáveis quando o buffer de saída está cheio (bit 0 do status ativo). Também devemos verificar erros de paridade e timeout:

~~~C
int read_KBC_output(uint8_t port, uint8_t *output) {
    uint8_t status;
    uint8_t attempts = 10;
    
    while (attempts > 0) {
        // Ler o status atual do KBC
        if (read_KBC_status(&status) != 0) {
            printf("Error: Status not available!\n");
            return 1;
        }

        // Verificar se o buffer de saída está cheio (há dados disponíveis)
        if ((status & BIT(0)) != 0) {
            // Ler o byte do buffer de saída
            if (util_sys_inb(port, output) != 0) {
                printf("Error: Could not read output!\n");
                return 1;
            }
            
            // Verificar erros de paridade ou timeout
            if ((status & BIT(7)) != 0) {
                printf("Error: Parity error!\n");
                return 1;  // Se houver erro, descarta o byte lido
            }
            if ((status & BIT(6)) != 0) {
                printf("Error: Timeout error!\n");
                return 1;  // Se houver erro, descarta o byte lido
            }
            
            return 0;  // Sucesso: byte lido sem erros
        }
        
        // Não há dados disponíveis, esperar e tentar novamente
        tickdelay(micros_to_ticks(20000));
        attempts--;
    }
    
    // Se chegamos aqui, todas as tentativas falharam
    return 1;
}
~~~

### Modos de operação do KBC

O KBC pode operar em dois modos principais:
- **modo de interrupções;**
- **modo de polling.**

<p align="center">
  <img src="../resources/images/Polling_Interrupts_versão1.png" alt="Polling vs. Interrupts" width="50%">
  <p align="center">Filha pergunta ao pai se já chegaram a todo o minuto. No exemplo de Interrupts, o pai diz à filha quando chegam e só aí é que ela acorda. Elaborado com IA.</p>

#### Modo de Interrupções

Neste modo, sempre que uma tecla é pressionada, o KBC gera uma interrupção. O processador interrompe a sua execução normal para processar essa interrupção através de um manipulador de interrupções (interrupt handler).

As interrupções oferecem um método eficiente para lidar com entradas do teclado, pois o processador só é interrompido quando realmente há dados a serem processados.

Tal como no timer, para usar este modo é preciso subscrever a interrupção. Como exemplo de implementação temos:
~~~C
#define KEYBOARD_IRQ 1 // Número de IRQ específico do teclado

int keyboard_hook_id = 1; // Identificador para a subscrição

// Subscrever interrupções do teclado
int keyboard_subscribe_int(uint8_t *bit_no) {
    if (bit_no == NULL) return 1; // O apontador deve ser válido
    
    *bit_no = BIT(keyboard_hook_id); // a função que chamou esta deve saber qual é a máscara a utilizar para detetar as interrupções gerais
    
    /* Subscrever em modo exclusivo:
    IRQ_REENABLE: reativa automaticamente as interrupções após cada interrupção
    IRQ_EXCLUSIVE: impede o handler padrão do Minix de processar estas interrupções */
    return sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id);
}

// Cancelar subscrição de interrupções
int keyboard_unsubscribe_int() {
    return sys_irqrmpolicy(&keyboard_hook_id);
}
~~~
O flag `IRQ_EXCLUSIVE` é crucial no Minix 3, pois impede que o manipulador de interrupções padrão do terminal seja notificado das interrupções do KBC, evitando interferência com o nosso driver.

#### Interrupt Handler (IH)

O manipulador de interrupções (IH) é uma função que será executada cada vez que o KBC gerar uma interrupção. No Minix, o IH deve seguir uma estrutura específica:
~~~C
void kbc_ih() {
    uint8_t status, scancode;
    
    // Ler o status do KBC
    if (read_KBC_status(&status) != 0) return;
    
    // Verificar se há dados disponíveis no buffer de saída e se são do teclado (não do rato)
    if ((status & BIT(0)) && !(status & BIT(5))) {
        // Ler o scancode do buffer de saída
        if (util_sys_inb(0x60, &scancode) != 0) return;
        
        // Verificar erros
        if ((status & BIT(7)) || (status & BIT(6))) {
            printf("Error: Parity or timeout error\n");
            // Descartar scancode em caso de erro
            return;
        }
        
        // Processar o scancode válido...
    }
}
~~~
É importante notar que o IH deve ser o mais simples e rápido possível, pois é executado no contexto de uma interrupção, bloqueando outras operações do sistema.

#### Modo de Polling

No modo de polling, o software verifica continuamente o estado do KBC para determinar se há novos dados a serem lidos. Embora menos eficiente em termos de uso do processador, este método é mais simples de implementar e útil em certas situações.
~~~C
int kbd_test_poll() {
    uint8_t scancode;
    uint8_t status;
    
    while(true) {  // Loop contínuo até condição de saída (ex: tecla ESC)
        // Verificar status do KBC
        if (read_KBC_status(&status) != 0) continue;
        
        // Se o buffer de saída tiver dados
        if (status & BIT(0)) {
            // Ler o scancode
            if (util_sys_inb(0x60, &scancode) != 0) continue;
            
            // Processar o scancode...
            
            // Verificar condição de saída (exemplo: ESC break code)
            if (scancode == 0x81) break;  // 0x81 é o break code da tecla ESC
        }
        
        // Pequena pausa para não sobrecarregar o CPU
        tickdelay(micros_to_ticks(20000));
    }
    
    // Antes de terminar, restaurar as interrupções do teclado
    kbc_restore();
    
    return 0;
}
~~~
É importante notar que, ao usar polling em sistemas como o Minix 3, é essencial restaurar o modo de interrupções antes de encerrar o programa, caso contrário, o teclado pode parar de responder até o sistema ser reiniciado:
~~~C
int kbc_restore() {
    uint8_t commandWord;
    
    // Ler a configuração atual do KBC
    if (write_KBC_command(0x64, 0x20) != 0) return 1;  // 0x20: comando para ler a palavra de comando
    if (read_KBC_output(0x60, &commandWord) != 0) return 1;
    
    // Ativar o bit de interrupções (bit 0)
    commandWord = commandWord | BIT(0);
    
    // Escrever a nova configuração
    if (write_KBC_command(0x64, 0x60) != 0) return 1;  // 0x60: comando para escrever a palavra de comando
    if (write_KBC_command(0x60, commandWord) != 0) return 1;
    
    return 0;
}
~~~
Este processo envolve:
1. Ler a palavra de comando atual do KBC;
2. Ativar o bit de interrupções (bit 0);
3. Escrever a palavra de comando modificada de volta ao KBC.

## 6. Tratamento de scancodes

No ponto 4 tivemos a oportunidade de ver como é distinguido um breakcode de um scancode, tal como uma possível implementação de código para fazer uma distinção rápida de um scancode e saber de que tipo é.

Algumas teclas especiais geram scancodes de dois bytes, começando normalmente com 0xE0. Para processá-los corretamente, é necessário manter estado entre leituras:
~~~C
// Variáveis globais para manter estado
bool two_byte_scancode = false;
uint8_t bytes[2];

void process_scancode(uint8_t byte) {
    if (two_byte_scancode) {
        // Este é o segundo byte de um scancode de dois bytes
        bytes[1] = byte;
        
        // Tratar o scancode completo de dois bytes
        bool make = !(byte & BIT(7));
        printf("Two-byte scancode: 0x%02x 0x%02x - %s\n", 
               bytes[0], byte, make ? "Make code" : "Break code");
               
        // Resetar o estado
        two_byte_scancode = false;
    } else if (byte == 0xE0) {
        // Este é o início de um scancode de dois bytes
        bytes[0] = byte;
        two_byte_scancode = true;
    } else {
        // Este é um scancode de um byte
        bool make = !(byte & BIT(7));
        printf("One-byte scancode: 0x%02x - %s\n", 
               byte, make ? "Make code" : "Break code");
    }
}
~~~
Este código diferencia entre scancodes de um e dois bytes, mantendo o estado necessário para processar corretamente sequências de bytes.

## 7. Contagem de chamadas a sys_inb()

Para fins de depuração e análise de desempenho, é útil contar quantas vezes o programa chama a função `sys_inb()`. Isto é especialmente importante para comparar os modos de polling e interrupção:
~~~C
// Contador global
unsigned int sys_inb_count = 0;

// Função para contar chamadas
int util_sys_inb(int port, uint8_t *value) {
    sys_inb_count++;
    return sys_inb(port, value);
}

// Função para obter o contador
unsigned int get_sys_inb_count() {
    return sys_inb_count;
}
~~~

Esta função permite que o programa saiba quantas vezes interagiu com o hardware, o que é valioso para avaliar a eficiência de diferentes abordagens. Por exemplo, o modo de polling resulta por norma num número muito superior de chamadas a `sys_inb()` do que o modo de interrupção.

## 8. Programando com múltiplos dispositivos

Um desafio adicional é lidar com interrupções de múltiplos dispositivos, como no teste `kbd_test_timed_scan()`, que utiliza tanto o teclado quanto o timer. Para isso, é necessário subscrever as interrupções de ambos os dispositivos e usar uma estrutura de tratamento adequada:
~~~C
int kbd_test_timed_scan(uint8_t idle_time) {
    uint8_t kbd_bit_no, timer_bit_no;
    int r, ipc_status;
    message msg;
    uint8_t scancode;
    unsigned int seconds = 0;
    
    // Subscrever interrupções do teclado e do timer
    if (keyboard_subscribe_int(&kbd_bit_no) != 0) return 1;
    if (timer_subscribe_int(&timer_bit_no) != 0) {
        keyboard_unsubscribe_int();
        return 1;
    }
    
    // Máscaras para identificar a origem das interrupções
    uint32_t kbd_irq_set = BIT(kbd_bit_no);
    uint32_t timer_irq_set = BIT(timer_bit_no);
    
    // Loop principal
    while (seconds < idle_time) {
        // Receber qualquer interrupção
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    // Interrupção do teclado
                    if (msg.m_notify.interrupts & kbd_irq_set) {
                        kbc_ih();  // Processar a interrupção do teclado
                        // Obter o scancode (implementação dependente)
                        scancode = get_scancode();
                        
                        // Processar o scancode...
                        
                        // Verificar se é o break code da tecla ESC
                        if (scancode == 0x81) {
                            // Sair do loop
                            seconds = idle_time + 1;  // Força saída do loop
                        } else {
                            // Reiniciar contador de tempo ocioso
                            seconds = 0;
                        }
                    }
                    
                    // Interrupção do timer
                    if (msg.m_notify.interrupts & timer_irq_set) {
                        timer_int_handler();  // Processar a interrupção do timer
                        
                        // Se passou um segundo...
                        if (timer_counter % 60 == 0) {  // Assumindo 60 interrupções/segundo
                            seconds++;  // Incrementar contador de tempo
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }
    
    // Cancelar subscrições antes de sair
    keyboard_unsubscribe_int();
    timer_unsubscribe_int();
    
    return 0;
}
~~~

Este código demonstra como tratar interrupções de múltiplos dispositivos. A função `driver_receive()` bloqueia até receber uma interrupção, e depois o programa identifica a fonte da interrupção usando máscaras de bits.

## 9. Compilação



## 11. Referências

1. Slides aulas teóricas de LCOM 2024/2025. Esses slides ficarão guardados na pasta resources/slides para referência futura (e porque, por vezes, os docentes gostam de ocultar o conteúdo do Moodle...)
2. 'Documentation for Lab3' - disponível na respetiva página web [aqui](https://pages.up.pt/~up722898/aulas/lcom2425/lab3/lab3.html)
3. Fabio Sá, repositório pessoal do [GitHub](https://github.com/Fabio-A-Sa/Y2S2-LabComputadores/tree/main/Labs/lab2#para-configurar-o-timer---configuration-command).