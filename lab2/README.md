# **Lab 2 - Timer**

## **1. Funções de teste esperadas**

Devemos implementar três funções de teste principais:
- **timer_test_read_config:** para ler e mostrar a configuração do timer;
- **timer_test_time_base:** para configurar um timer com uma frequência específica;
- **timer_test_int:** para testar a gestão de interrupções do timer (vamos ver o que é isso mais adiante)

Para implementar as duas primeiras funções recomendo a leitura atenta na íntegra dos pontos 2 a 5.

Para a última função, não só é necessário compreender como funciona a comunicação com os timers como também é preciso perceber o que são interrupções e como são abordadas. Para tal recomendo a leitura do [ponto 6](https://github.com/tiagoleic02/LCOM/tree/master/lab2#6-interrup%C3%A7%C3%B5es).

## **2. Funcionamento do Timer (i8254)**

### **📌 O que é?**

O temporizador do computador, conhecido como i8254, é um dos componentes de _‘hardware’_ mais simples que conseguimos programar em linguagem C.

### **🧱 Estrutura do i8254**

O temporizador tem 3 contadores de 16 bits (uint16_t), cada um com uma função específica:
~~~lua
+------------+------------+-------------------------------------------+
|   Timer    |  Endereço  |                 Função                    |
+------------+------------+-------------------------------------------+
|  Timer 0   |    0x40    | Fornece uma base de tempo ao sistema      |
|  Timer 1   |    0x41    | Faz refresh da memória DRAM               |
|  Timer 2   |    0x42    | Gera tons/frequências para os speakers    |
+------------+------------+-------------------------------------------+
|  Control   |    0x43    | Registo de controlo (envio de comandos)   |
+------------+------------+-------------------------------------------+
~~~
### **💬 Comunicação com o i8254 em C**

Quando falamos em comunicar com o timer, falamos em duas hipóteses:
- Receber informações do timer;
- Enviar informações (alterar configurações) do timer;

Em qualquer comunicação com o timer temos **sempre** que informar o registo de controlo (0x43) para ele 'ficar a contar' com o que pretendemos fazer. Só após o registo de controlo ter do lado dele a informação daquilo que pretendemos fazer (enviar ou receber a configuração de um timer especifico p.e.) é que podemos efetivamente recolher ou enviar essa informação.

Para isso, usamos duas system calls:

#### 🔽 _sys_outb_ — **Envia comandos/informações para o timer**
~~~C
int sys_outb(uint8_t port, uint32_t command);
~~~
**Para que serve:** enviar configurações para os timers (por exemplo, definir como contam ou com que frequência) ou para o registo de controlo.

**Exemplo:**
~~~C
sys_outb(0x43, 0x36); // Envia um comando de configuração para o registo de controlo
sys_outb(0x40, 0x9C); // Define parte do valor inicial no Timer 0
~~~
#### 🔼 _sys_inb_ — **Lê informação do timer**
~~~C
int sys_inb(uint8_t port, uint32_t *value);
~~~
**Para que serve:** consultar o valor atual de um dos timers.

**Exemplo:**
~~~C
uint32_t val;
sys_inb(0x40, &val);  // Lê o valor atual do Timer 0 e guarda em val
~~~

### Nota #1 - Função _util_sys_inb_:

Repara que o comando sys_inb que lê informações do timer recebe um valor por um apontador de 32 bits. No entanto, é dispensável esses 32 bits, pois no contexto de LCOM apenas são necessários 8 e essa diferença leva muitas vezes a erros desnecessários.
> **_Qual a alternativa para evitar esses erros?_** --> Implementar uma função auxiliar que receba esse apontador e converta em 8 bits
~~~C
int (util_sys_inb)(int port, uint8_t *value) {
  if(value == NULL) return 1;
  uint32_t temp;
  int result = sys_inb(port, &temp); //Chamada do sys_inb para ler da porta
  *value = (uint8_t)temp; //Converter o valor de temp para uint8_t - é este o objetivo desta função!
  return result; //retornar para esta nova função o mesmo valor que a função original também retornava
}
~~~

### Nota #2 - Configuração da frequência do timer usando MSB e LSB [*1](https://github.com/tiagoleic02/LCOM/tree/master/lab2#refer%C3%AAncias)
Como já deves ter percebido, no caso de querermos alterar a configuração/frequência de um timer, após passar a informação para o registo de controlo sobre a alteração que pretendemos fazer, é necessário injetar o valor inicial no timer da porta correspondente (0x40, 0x41 ou 0x42).

Cada contador tem um valor interno que é decrementado conforme a frequência do CPU. No caso do MINIX é decrementado 1193182 vezes por segundo. Sempre que o valor do contador fica a 0 o dispositivo notifica o CPU (gera uma interrupção, veremos no ponto 6 o que é) e volta ao valor original.

Por exemplo, para um CPU de frequência 100 Hz e um Timer de 4 Hz precisamos de ter o contador com valor 25. Esquema ilustrativo:
<p align="center">
  <img src="../resources/images/Counter.png" alt="gráfico do calculo do contador">
  <p align="center">Cálculo do valor do contador interno (ver referência 1)</p>

Para alterar a frequência do timer selecionado, de modo a conseguirmos por exemplo contar segundos (com uma frequência de 60Hz) através das interrupções geradas, devemos calcular o valor interno.

~~~C
#define TIMER_FREQUENCY 1193182
uint16_t frequency = 60;
uint16_t counter = TIMER_FREQUENCY / frequency;
~~~

Para configurar o timer com este valor, temos que usar o _**sys_outb**_. Mas essa função só aceita valores de 8 bits...

> **_Então porque não definimos a variável counter como uma variável de 8 bits?_** 
Porque não é possivel alojar um número tão grande como uma divisão de 1193182 por 60 em apenas 8 bits!

Por esse mesmo motivo é que não só temos que separar os 16 bits em 8+8 (ou **MSB+LSB**) como ainda temos que garantir que o valor da frequency nunca é menor do que 19 (pois se for 18 counter é 66288 e o máximo suportado por uma variável de 16 bits é 65535). 

Para enviar os 8 bits mais significativos (Most Significant Bits ou MSB) seguido dos 8 bits menos significativos (Less Significant Bits ou LSB), temos que definir duas funções para o efeito. Estas devem ser incluídas no utils.c.
~~~C
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb == NULL) return 1; //Verificar se o lsb é válido
  *lsb = (uint8_t)(val & 0xFF);  //Passar para o lsb os 8 bits menos significativos de val; podemos omitir 0xFF
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == NULL) return 1;
  *msb = (uint8_t)((val >> 8) & 0xFF); //Deslocar 8 bits e passar tal como no lsb os restantes 8 bits do val para o msb
  return 0;
}
~~~
O resto é feito pelo _Configuration Command_, que veremos no ponto seguinte o que faz e de que forma envia o MSB + LSB.

### Resumindo...

- Cada timer tem o seu endereço (0x40, 0x41, 0x42);
- O registo 0x43 serve para dizer ao temporizador como se deve comportar;
- Usamos:
  - sys_outb() → para escrever nos timers ou no registo de controlo;
  - sys_inb() → para ler os valores dos timers.
- O temporizador é independente da velocidade do processador, o que permite medir o tempo com fiabilidade.

## **3. Programação do Timer**

Para programar um timer é necessário:
- **Escrever um código de 8 bits no registo de controlo 0x43** (especificando o modo de operação);
- **Carregar o valor inicial do contador;**

A palavra de controlo inclui:
- Bits 7,6: Seleção do contador (00 para timer 0, 01 para timeer 1, 10 para timer 2);
- Bits 5,4: Modo de inicialização (01 só LSB, 10 só MSB, 11 LSB seguido de MSB*);
- Bits 3, 2, 1: Modo de operação (011 para modo 3 que será o que vamos usar maioritariamente);
- Bit 0: Base de contagem (0 para binário, 1 para BCD).

### _Configuration Command:_
~~~lua
+-------+---------+---------------------------+
|  Bit  |  Value  |         Função            |
+-------+---------+---------------------------+
|  7,6  |         |     Counter selecion      |
+-------+---------+---------------------------+
|       |    00   |            0              |
|       |    01   |            1              |
|       |    10   |            2              |
+-------+---------+---------------------------+
|  5,4  |         |   Counter initialization  |
+-------+---------+---------------------------+
|       |    01   |           LSB             |
|       |    10   |           MSB             |
|       |    11   |    LSB folowed by MSB     |
+-------+---------+---------------------------+
| 3,2,1 |         |   Counter initialization  |
+-------+---------+---------------------------+
|       |   000   |            0              |
|       |   001   |            1              |
|       |   x10   |            2              |
|       |   x11   |            3              |
|       |   100   |            4              |
|       |   101   |            5              |
+-------+---------+---------------------------+
|   0   |         |           BCD             |
+-------+---------+---------------------------+
|       |    0    |     Binary (16 bits)      |
|       |    1    |      BCD (4 digits)       |
+-------+---------+---------------------------+
~~~

> Como devemos proceder para alterar a frequência de um timer usando um valor de 16 bits?

Tal como já foi visto na **Nota #2**, irei agora mostrar na prática como se processa todos os cálculos, incluindo a aplicação do "LSB followed by MSB" no _configuration command_.
~~~C
  //Ler a configuração atual para preservar alguns bits
  uint8_t st;
  if ((timer_get_conf(timer, &st)) != 0) return 1;

  // Calcular o valor de contagem baseado na frequência
  uint16_t initial_count = TIMER_FREQ / freq;

  // Preparar o comando para configurar o timer
  uint8_t ctrl_word = (st & 0x0F) | TIMER_LSB_MSB;  // Preservar os 4 bits inferiores e definir modo de acesso

  // Selecionar o timer correto
  switch (timer) {
    case 0: ctrl_word |= TIMER_SEL0; break;
    case 1: ctrl_word |= TIMER_SEL1; break;
    case 2: ctrl_word |= TIMER_SEL2; break;
  }
  //Enviar o comando para o registo de controlo
  if ((sys_outb(TIMER_CTRL, ctrl_word)) != 0) return 1;

  //Obter o LSB e MSB do valor de contagem
  uint8_t lsb, msb;
  util_get_LSB(initial_count, &lsb);
  util_get_MSB(initial_count, &msb);

  //Enviar o valor de contagem para o timer
  if ((sys_outb(TIMER_0 + timer, lsb)) != 0) return 1;
  if ((sys_outb(TIMER_0 + timer, msb)) != 0) return 1;
~~~

Como penso ser óbvio, devem implementar uma configuração semelhante a esta no timer_set_frequency.

## **4. Leitura da configuração**

Tal como quando vamos escrever, para ler a configuração de um timer é necessário usar o comando Read-Back.
Para ler temos então que:
1. _Escrever o comando Read-Back no registo de controlo._

### _Read-Back Command Format_
~~~lua
+-------+---------+-----------------------------------+
|  Bit  |  Value  |              Função               |
+-------+---------+-----------------------------------+
|  7,6  |         |         Read-Back Command         |
+-------+---------+-----------------------------------+
|       |    11   |                                   |
+-------+---------+-----------------------------------+
|   5   |         |              COUNT                |
+-------+---------+-----------------------------------+
|       |    0    |        Read counter value         |
+-------+---------+-----------------------------------+
|   4   |         |              STATUS               |
+-------+---------+-----------------------------------+
|       |    0    |       Read programmed mode        |
+-------+---------+-----------------------------------+
|   3   |         |          Select Timer 2           |
+-------+---------+-----------------------------------+
|       |    1    |                Yes                |
+-------+---------+-----------------------------------+
|   2   |         |          Select Timer 1           |
+-------+---------+-----------------------------------+
|       |    1    |                Yes                |
+-------+---------+-----------------------------------+
|   1   |         |          Select Timer 0           |
+-------+---------+-----------------------------------+
|       |    1    |                Yes                |
+-------+---------+-----------------------------------+
|   0   |         |             Reserved              |
+-------+---------+-----------------------------------+
~~~
2. _Ler os 8 bits do timer selecionado através do comando **sys_inb**._

## **5. Implementação**

Já vimos toda a base que precisamos para implementar as duas primeiras funções referidas no ponto 1.

Apesar de ambas virem pré-definidas no lab2.c, para que as mesmas funcionem é preciso implementar funções importantes, como:
~~~C
//Para obter a configuração atual de um dado timer
int (timer_get_conf)(uint8_t timer, uint8_t *st)
//Para mostrar a configuração atual de um dado timer, traduzindo a configuração obtida interpretando cada um dos bits segundo a tabela de Read-Back Command
int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field)
//Para alterar a configuração (frequência) de um dado timer 
int (timer_set_frequency)(uint8_t timer, uint32_t freq)
~~~
Mas como fazemos para implementar cada uma destas funções?

## **6. Interrupções**

Para ativar as interrupções é necessário subscrevê-las por meio de uma system call e antes de acabar o programa deve-se desligar as interrupções usando outra, para garantir a reposição do estado inicial da máquina. Por norma o bit de interrupção é definido pelo módulo que gere o próprio dispositivo, para que seja independente do programa.

### Algumas notas sobre a função sys_irqsetpolicy:
Estrutura
~~~C
int sys_irqsetpolicy(int irq, int policy, int *hook_id);_
~~~
**1. irq:**

    Significado: é o número da linha IRQ (Interrupt Request) a ser tratada; Pode ir de 0 a 15;
Valores comuns:
- 0: Timer do sistema;
- 1: Teclado
- 3: Porta serial COM2
- 4: Porta serial COM1
- 8: Relógio de tempo real
- 12: Mouse PS/2

Usamos TIMER0_IRQ (que é 0, está definido no i8254)

**2. policy (int)**

    Significado: Indica como o sistema deve lidar com a interrupção;

Opções comuns:
- IRQ_REENABLE: Reativa automaticamente a linha de interrupção após cada tratamento;
- IRQ_EXCLUSIVE: Solicita uso exclusivo da linha de interrupção;
- IRQ_REENABLE | IRQ_EXCLUSIVE: Combina ambas as políticas;
- IRQ_REENABLE_NOTIFICATION: Ativa uma notificação centralizada para coordenar múltiplas interrupções;

Usamos a primeira opção para que o timer continue a gerar interrupções automaticamente após cada interrupção ser tratada.

**3.hook_id**

    Significado: é um identificador usado pelo sistema para rastrear a assinatura de interrupção especifica.
Uso:
- Entrada/Saída: É um parâmetro passado por referência;
- Antes de ser chamado: Inicializamos com o número da IRQ (hook_id = TIMER0_IRQ);
- Após ser chamado: O sistema modifica o valor para um ID único.

Importância:
- É necessário para desativar a interrupção posteriormente (em sys_irqrmpolicy());
- É usado para identificar qual o dispositivo que gerou uma interrupção (quando múltiplos dispositivos compartilham o mesmo IRQ);
- Serve como um ‘token’ que conecta o manipulador ao sistema de interrupções;

## Referências:

1. Fabio Sá, repositório pessoal do [GitHub](https://github.com/Fabio-A-Sa/Y2S2-LabComputadores/tree/main/Labs/lab2#para-configurar-o-timer---configuration-command). A informação presente na nota 2 é da autoria do Fábio tendo apenas sido **adaptada** por mim.