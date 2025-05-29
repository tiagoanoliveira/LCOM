# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/drivers/
.PATH: ${.CURDIR}/drivers/graphics
.PATH: ${.CURDIR}/drivers/keyboardMouse
.PATH: ${.CURDIR}/drivers/timer
.PATH: ${.CURDIR}/drivers/utils
.PATH: ${.CURDIR}/video
.PATH: ${.CURDIR}/game
.PATH: ${.CURDIR}/game/core
.PATH: ${.CURDIR}/game/objects
.PATH: ${.CURDIR}/game/states
.PATH: ${.CURDIR}/game/ui


# source code files to be compiled
DRIVER_SRCS = graphics.c timer.c keyboard.c mouse.c KBC.c utils.c
CORE_SRCS = game.c tetris.c input.c
OBJECT_SRCS = piece.c score.c highscore.c
STATES_SRCS = state.c menu_state.c gameover_state.c
UI_SRCS = font.c render.c menu_ui.c gameover_ui.c

SRCS = main.c $(DRIVER_SRCS) $(CORE_SRCS) $(OBJECT_SRCS) $(STATES_SRCS) $(UI_SRCS)

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -D_LCOM_OPTIMIZED_

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf -lxpm -llm

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
