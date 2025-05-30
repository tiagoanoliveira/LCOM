#ifndef CONFIG_H
#define CONFIG_H

#include "../../../drivers/graphics/graphics.h"

// Configurações do jogo
#define DROP_SPEED_INITIAL 60  // frames
#define DROP_SPEED_FAST 5      // frames

// Configurações gráficas
#define SCREEN_MODE 0x105

// Moldura externa do jogo
#define COLOR_BORDER 0x03
#define FRAME_THICKNESS 5
#define SCREEN_BORDER_WIDTH 10

// Cores do Menu, Jogo e GameOver
#define COLOR_BACKGROUND 0x00
#define COLOR_TITLE 0x0F
#define COLOR_SELECTED 0x3E
#define COLOR_NORMAL 0x07
#define COLOR_SCORE 0x0A

// Cores da área de jogo
#define GRID_BORDER_COLOR 0x38     // cinzento
#define GRID_BORDER_LINES_COLOR 0x00
#define GRID_BACKGROUND_COLOR 0x00  // fundo da grelha
#define GRID_COLOR 0x07

// Configurações da grid
#define GRID_COLS 10
#define GRID_ROWS 20
#define CELL_SIZE 30

// Dimensões e Posições da grelha
#define GRID_WIDTH (GRID_COLS * CELL_SIZE)
#define GRID_HEIGHT (GRID_ROWS * CELL_SIZE)
#define GRID_WITH_BORDER_WIDTH (GRID_WIDTH + 2 * CELL_SIZE)
#define GRID_WITH_BORDER_HEIGHT (GRID_HEIGHT + 2 * CELL_SIZE)
#define GRID_WITH_BORDER_ORIGIN_X (grid_origin_x - CELL_SIZE)
#define GRID_WITH_BORDER_ORIGIN_Y (grid_origin_y - CELL_SIZE)

// Dimensões, Cores e Posições do Score, Lines_Cleaned, TopScore, NextPiece e Level
#define LEFT_INFOS_WIDTH 200  // Largura da caixa do Score, Lines_Cleaned e Top Score
#define RIGHT_INFOS_WIDTH 200   // Largura da caixa da Next Piece e Level
#define SCORE_HEIGHT 100
#define LINES_CLEANED_HEIGHT 100
#define TOP_SCORE_HEIGHT 200
#define NEXT_PIECE_HEIGHT 140
#define LEVEL_HEIGHT 100
#define LEFT_INFOS_ORIGIN_X ((GRID_WITH_BORDER_ORIGIN_X-LEFT_INFOS_WIDTH+SCREEN_BORDER_WIDTH-FRAME_INFOS_THICKNESS)/2)
#define SCORE_ORIGIN_Y GRID_WITH_BORDER_ORIGIN_Y
#define FRAME_INFOS_COLOR 0x04
#define TEXT_INFOS_COLOR 0x07
#define FRAME_INFOS_THICKNESS 5

#define SCORE_BG_COLOR 0x3E
#define LINES_BG_COLOR 0x0A
#define TOP_SCORE_BG_COLOR 0x3E
#define NEXT_PIECE_BG_COLOR 0x3E
#define LEVEL_BG_COLOR 0x3E

// Font dimensions
#define CHAR_WIDTH 16
#define CHAR_HEIGHT 24

// Title font dimensions
#define TITLE_SCALE 3
#define TITLE_CHAR_WIDTH  (CHAR_WIDTH * TITLE_SCALE)
#define TITLE_CHAR_HEIGHT (CHAR_HEIGHT * TITLE_SCALE)

// Cores das peças
#define COLOR_I 0x03  // Cyan
#define COLOR_O 0x04  // Red
#define COLOR_T 0x05  // Rose
#define COLOR_S 0x0A  // Green
#define COLOR_Z 0x3E  // Yellow
#define COLOR_J 0x0B  // Blue
#define COLOR_L 0x2E  // Orange

#endif //CONFIG_H
