


> Quais os conhecimentos-base que devo ter para este lab?

Em príncipio, se dominaste todos os conceitos descritos no [Lab2](https://github.com/tiagoleic02/LCOM/blob/master/lab2/README.md) não terás grandes dificuldades em realizar este lab, principalmente se tiveres mantido o teu código limpo e organizado.
Para este lab precisas:
- Compreender como funciona a comunicação entre um programa/driver e o controlador do dispositivo I/O, incluindo:
  - Utilização de funções _sys_inb_ e _sys_outb_;
  - Necessidade de usar uma função auxiliar que converta os 32 bits do apontador usado em _sys_inb_ em 8 bits;
- Usar um registo de controlo para informar o controlador do que pretendemos acessar ou modificar antes;