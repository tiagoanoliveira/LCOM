# Lab 4 - The PS/2 Mouse

## 1. Funções de teste esperadas

Neste laboratório é esperado que implementes as seguintes funções de teste principais:
- **`mouse_test_packet`** -
- **`mouse_test_async`** -
- **`mouse_test_gesture`** -

# 🚧🚧🚧 EM CONSTRUÇÃO 🚧🚧🚧

## 2. Ponto de partida: ficheiros necessários

A construção dos labs anteriores de forma modular permitiu que a esmagadora maioria dos ficheiros necessários para este lab já existam e estejam prontos a usar. São eles:
- **`KBC.c`** e **`KBC.h`**;
- **`timer.c`** e **`timer.h`**;
- **`utils.c`**;
- **`i8254.h`**;

Além disso, o ficheiro **`i8042.h`** será reutilizado, ao qual iremos acrescentar os **comandos do rato**, as **respostas do rato** e as **máscaras dos bits** do primeiro byte do pacote do rato. Deves também adicionar o **MOUSE_IRQ** que será 12.

Adicionalmente irás precisar de criar o ficheiro mouse.c com as seguintes funções:
- `mouse_subscribe_int` - Subscreve as interrupções do rato em modo exclusivo;
- `mouse_unsubscribe_int` - Cancela a subscrição de interrupções;
- `mouse_ih` - Interrupt handler para processar interrupções do rato;
- `mouse_sync_bytes` - Avalia a disposição dos bytes num array pré-definido;
- `mouse_bytes_to_packet` - Transforma o array de bytes numa struct definida conforme a documentação;
- `mouse_write_cmd` - Escreve um comando no KBC, lidando com tentativas e atrasos. 

Neste [link](https://github.com/tiagoleic02/LCOM/tree/master/Modelos/lab4) tens o molde deste lab que podes descarregar para trabalhar a partir do mesmo. Nele podes encontrar:
- Ficheiros i8254.h, timer.c, KBC.c e utils.c do lab2 e lab3 funcionais;
- Ficheiro i8042.h já completado com as definições necessárias para este lab conforme previsto na documentação de apoio fornecida;
- Ficheiros acima descritos com os `#includes` já prontos e funções que considero necessárias declaradas;
- Todos os respetivos ficheiros `.h` que considero necessários para nada falhar e não haver conflitos ou erros inesperados;
- `lab4.c` conforme é fornecido nos documentos de LCOM;

Trabalha nos ficheiros que te forneci (`.c`) seguindo o guião deste laboratório e, se tiveres alguma dúvida ou sugestão de melhoria, abre uma [discussão](https://github.com/tiagoleic02/LCOM/discussions/new/choose).

## 3. Como funciona o rato?

# 🚧🚧🚧 EM CONSTRUÇÃO 🚧🚧🚧


