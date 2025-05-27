//
// Created by tiago-oliveira on 27-05-2025.
//

#ifndef CONFIG_H
#define CONFIG_H

// Configurações do jogo
#define GAME_FPS 60
#define DROP_SPEED_INITIAL 60  // frames
#define DROP_SPEED_FAST 6      // frames

// Configurações gráficas
#define SCREEN_MODE 0x105
#define BACKGROUND_COLOR 0x00

// Configurações da grid
#define GRID_COLS 10
#define GRID_ROWS 20
#define CELL_SIZE 30

// Cores das peças
#define COLOR_I 0x03  // Cyan
#define COLOR_O 0x04  // Yellow
#define COLOR_T 0x05  // Purple
#define COLOR_S 0x06  // Green
#define COLOR_Z 0x07  // Red
#define COLOR_J 0x08  // Blue
#define COLOR_L 0x09  // Orange

// Cores do Menu e GameOver
static const uint32_t COLOR_BACKGROUND = 0x00;
static const uint32_t COLOR_TITLE = 0x0F;
static const uint32_t COLOR_SELECTED = 0x0E;
static const uint32_t COLOR_NORMAL = 0x07;
static const uint32_t COLOR_BORDER = 0x03;
static const uint32_t COLOR_SCORE = 0x0A;

#endif //CONFIG_H
