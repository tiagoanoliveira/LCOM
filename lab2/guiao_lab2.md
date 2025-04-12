# **Lab 2 - Timer**

## 1. Funções de teste esperadas

Devemos implementar três funções de teste principais:
- **timer_test_read_config:** para ler e mostrar a configuração do timer;
- **timer_test_time_base:** para configurar um timer com uma frequência específica;
- **timer_test_int:** para testar a gestão de interrupções do timer (vamos ver o que é isso mais à frente)

## 2. Funcionamento do Timer (i8254)

#### **📌 O que é?**

O temporizador do computador, conhecido como i8254, é um dos componentes de hardware mais simples que conseguimos programar em linguagem C.

#### **🧱 Estrutura do i8254**

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
#### **💬 Comunicação com o i8254 em C**

Para enviar ou receber dados destes registos, usamos duas system calls:

🔽 _sys_outb_ — **Envia comandos/informações para o timer**
~~~C
int sys_outb(uint8_t port, uint32_t command);
~~~
**Para que serve:** enviar configurações para os timers (por exemplo, definir como contam ou com que frequência).

**Exemplo:**
~~~C
sys_outb(0x43, 0x36); // Envia um comando de configuração para o registo de controlo
sys_outb(0x40, 0x9C); // Define parte do valor inicial no Timer 0
~~~


## 3. Programação do Timer

Para programar um timer é necessário:
- **Escrever um código de 8 bits no registo de controlo 0x43** (especificando o modo de operação);
- **Carregar o valor inicial do contador;**

A palavra de controlo inclui:
- Bits 7,6: Seleção do contador (00 para timer 0, 01 para timeer 1, 10 para timer 2);
- Bits 5,4: Modo de inicialização (01 só LSB, 10 só MSB, 11 LSB seguido de MSB*);
- Bits 3, 2, 1: Modo de operação (011 para modo 3 que será o que vamos usar maioritariamente);
- Bit 0: Base de contagem (0 para binário, 1 para BCD).



## **INTERRUPÇÕES**

Para ativar as interrupções é necessário subscrevê-las através de uma system call e antes de acabar o programa deve-se
desligar as interrupções usando outra, para garantir a reposição do estado inicial da máquina. Por norma o bit de
interrupção é definido pelo módulo que gere o próprio dispositivo, para que seja independente do programa.

### Algumas notas sobre a função sys_irqsetpolicy:
Estrutura
~~~cpp
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
- Depois de chamado: O sistema modifica o valor para um ID único.

Importância:
- É necessário para desativar a interrupção posteriormente (em sys_irqrmpolicy());
- É usado para identificar qual o dispositivo que gerou uma interrupção (quando múltiplos dispositivos compartilham o mesmo IRQ);
- Serve como um token que conecta o manipulador ao sistema de interrupções;

