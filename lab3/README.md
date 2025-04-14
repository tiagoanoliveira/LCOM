# Lab 3 - Keyboard

## 1. Funções de teste esperadas

Neste laboratório é esperado que implementes as seguintes funções de teste principais:
- **kbd_test_scan** - Leitura de scancodes usando um handler de interrupções em C;
- **kbd_test_poll** - Leitura de scancodes via polling (sem interrupções);
- **kbd_test_timed_scan** - Combinar os handlers de interrupções do teclado e do timer.


## 2. Criar o ficheiro i8042

No ponto 3.2. da documentação de apoio para o [Lab3](https://pages.up.pt/~up722898/aulas/lcom2425/lab3/lab3_03.html) é fornecido uma descrição funcional do i8042, incluindo com detalhe todos os comandos possíveis do mesmo. Como é expectável, não precisamos de todos, daí que deixo abaixo o conteúdo necesário para o [i8042.h](https://github.com/tiagoleic02/LCOM/blob/master/lab3/i8042.h):
~~~C

~~~
...

> Quais os conhecimentos-base que devo ter para este lab?

Em príncipio, se dominaste todos os conceitos descritos no [Lab2](https://github.com/tiagoleic02/LCOM/blob/master/lab2/README.md) não terás grandes dificuldades em realizar este lab, principalmente se tiveres mantido o teu código limpo e organizado.
Para este lab precisas:
1. [x] Compreender como funciona a comunicação entre um programa/driver e o controlador do dispositivo I/O, incluindo:
      1. [x] Utilização de funções _sys_inb_ e _sys_outb_;
      2. [x] Necessidade de usar uma função auxiliar que converta os 32 bits do apontador usado em _sys_inb_ em 8 bits;
2. [x] Usar um registo de controlo para informar o controlador do que pretendemos acessar ou modificar antes;