#include "tetris.h"
#include "render.h"

void tetris_init() {
    // Inicialização futura: grid, peça ativa, pontuação...
}

void tetris_draw() {
    vg_clear_screen(0x00); // fundo preto
    draw_grid();           // grelha Tetris
}
