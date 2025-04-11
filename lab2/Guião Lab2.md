# **Lab 2 - Timer**

## 1. Funções de teste esperadas

Devemos implementar três funções de teste principais:
- **timer_test_read_config:** para ler e mostrar a configuração do timer;
- **timer_test_time_base:** para configurar um timer com uma frequência específica;
- **timer_test_int:** para testar a gestão de interrupções do timer (vamos ver o que é isso mais à frente)

## 2. Funcionamento do Timer (i8254)

O PC possui um circuito integrado i8254 com 3 timers independentes. Cada timer tem um contador de 16 bits que é decrementado a cada pulso de clock. Neste laboratório vamos utilizar principalmente o timer 0 e o modo de operação 3 (gerador de onda quadrada).
Os endereços de I/O do i8254 são: 0x43 (registo de controlo) e 0x40, 0x41, 0x42 (registo para os timers 0, 1 e 2).

## 3. Programação do Timer

Para programar um timer é necessário:
- Escrever uma palavra de controlo no registo de controlo (especificando o modo de operação);
- Carregar o valor inicial do contador;



.

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

