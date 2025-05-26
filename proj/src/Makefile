# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/drivers/
.PATH: ${.CURDIR}/drivers/graphics
.PATH: ${.CURDIR}/drivers/keyboardMouse
.PATH: ${.CURDIR}/drivers/timer
.PATH: ${.CURDIR}/drivers/utils
.PATH: ${.CURDIR}/video
.PATH: ${.CURDIR}/game

# source code files to be compiled
SRCS = main.c \
       graphics.c \
       timer.c \
       keyboard.c \
       mouse.c \
       KBC.c \
       utils.c \
       tetris.c \
       piece.c \
       render.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -D_LCOM_OPTIMIZED_

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf -lxpm -llm

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
