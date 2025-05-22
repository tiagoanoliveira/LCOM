#ifndef PIECE_H
#define PIECE_H

#include <stdint.h>
#include <stdbool.h>

#define PIECE_SIZE 4 // Tetris pieces are 4x4

typedef enum {
    PIECE_I, PIECE_O, PIECE_T, PIECE_S, PIECE_Z, PIECE_J, PIECE_L, PIECE_NONE
} PieceType;

typedef struct {
    PieceType type;
    int rotation; // 0-3
    int x, y;     // Position on the grid (top-left of 4x4 matrix)
    uint8_t shape[4][4]; // Current rotation shape
} Piece;

// Initialize a piece of a given type at a given position
void piece_init(Piece *piece, PieceType type, int x, int y);

// Rotate the piece (clockwise or counterclockwise)
void piece_rotate(Piece *piece, bool clockwise);

// Copy the shape for the current rotation into piece->shape
void piece_update_shape(Piece *piece);

// Get the color for a given piece type
uint32_t piece_get_color(PieceType type);

#endif // PIECE_H