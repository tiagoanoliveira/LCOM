#include "piece.h"
#include "render.h"

// Define the 4 rotations for each piece type (4x4 matrices)
static const uint8_t PIECE_SHAPES[7][4][4][4] = {
    // PIECE_I
    {
        { {0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0} },
        { {0,0,1,0}, {0,0,1,0}, {0,0,1,0}, {0,0,1,0} },
        { {0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0} },
        { {0,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,1,0,0} }
    },
    // PIECE_O
    {
        { {0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0} },
        { {0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0} },
        { {0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0} },
        { {0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0} }
    },
    // PIECE_T
    {
        { {0,1,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0} },
        { {0,1,0,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0} },
        { {0,0,0,0}, {1,1,1,0}, {0,1,0,0}, {0,0,0,0} },
        { {0,1,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0} }
    },
    // PIECE_S
    {
        { {0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0} },
        { {0,1,0,0}, {0,1,1,0}, {0,0,1,0}, {0,0,0,0} },
        { {0,0,0,0}, {0,1,1,0}, {1,1,0,0}, {0,0,0,0} },
        { {1,0,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0} }
    },
    // PIECE_Z
    {
        { {1,1,0,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0} },
        { {0,0,1,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0} },
        { {0,0,0,0}, {1,1,0,0}, {0,1,1,0}, {0,0,0,0} },
        { {0,1,0,0}, {1,1,0,0}, {1,0,0,0}, {0,0,0,0} }
    },
    // PIECE_J
    {
        { {1,0,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0} },
        { {0,1,1,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0} },
        { {0,0,0,0}, {1,1,1,0}, {0,0,1,0}, {0,0,0,0} },
        { {0,1,0,0}, {0,1,0,0}, {1,1,0,0}, {0,0,0,0} }
    },
    // PIECE_L
    {
        { {0,0,1,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0} },
        { {0,1,0,0}, {0,1,0,0}, {0,1,1,0}, {0,0,0,0} },
        { {0,0,0,0}, {1,1,1,0}, {1,0,0,0}, {0,0,0,0} },
        { {1,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0} }
    }
};

static const uint32_t PIECE_COLORS[7] = {
    0x03, // I - Cyan
    0x04, // O - Yellow
    0x05, // T - Purple
    0x06, // S - Green
    0x07, // Z - Red
    0x08, // J - Blue
    0x09  // L - Orange
};

void piece_init(Piece *piece, PieceType type, int x, int y) {
    piece->type = type;
    piece->rotation = 0;
    piece->x = x;
    piece->y = y;
    piece->color = PIECE_COLORS[type];
    piece_update_shape(piece);
}

void piece_rotate(Piece *piece, bool clockwise) {
    if (clockwise)
        piece->rotation = (piece->rotation + 1) % 4;
    else
        piece->rotation = (piece->rotation + 3) % 4;
    piece_update_shape(piece);
}

void piece_update_shape(Piece *piece) {
    if (piece->type >= PIECE_I && piece->type <= PIECE_L) {
        for (int i = 0; i < PIECE_SIZE; i++)
            for (int j = 0; j < PIECE_SIZE; j++)
                piece->shape[i][j] = PIECE_SHAPES[piece->type][piece->rotation][i][j];
    }
}

uint32_t piece_get_color(PieceType type) {
    if (type >= PIECE_I && type <= PIECE_L)
        return PIECE_COLORS[type];
    return 0xFFFFFF; // Default: white
}

bool piece_fits(const Piece* piece, int new_x, int new_y) {
    for (int i = 0; i < PIECE_SIZE; i++) {
        for (int j = 0; j < PIECE_SIZE; j++) {
            if (piece->shape[i][j]) {
                int gx = new_x + j;
                int gy = new_y + i;
                if (gx < 0 || gx >= GRID_COLS || gy < 0 || gy >= GRID_ROWS)
                    return false;
            }
        }
    }
    return true;
}
