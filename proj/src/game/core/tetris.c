#include "tetris.h"
#include "../ui/render.h"
#include "../../drivers/keyboardMouse/keyboard.h"

static int land_counter = 0;
static int lock_resets = 0;
static int fall_tick_counter = 0;
static bool is_landed = false;
static bool moved_this_tick = false;
const int MAX_LOCK_RESETS = 5;

static bool moved_in_logic_tick = false;
static bool movement_allowed = true;

static int logic_counter = 0;
const int LOGIC_TICK_INTERVAL = 1; // 1 = run logic_tick every game_tick (you can change to 2 later)

int grid[GRID_ROWS][GRID_COLS] = {{0}}; // 0 means empty cell

// TODO: Check tick logic as it is still not forcing max 5 lock resets, it's better but not 100%

void tetris_init() {
    // Clear grid
    for (int r = 0; r < GRID_ROWS; r++) {
        for (int c = 0; c < GRID_COLS; c++) {
            grid[r][c] = 0;
        }
    }

    // Initialize current piece
    PieceType type = random_piece_type();
    piece_init(&current_piece, type, 4, 0); // center top
}

void tetris_draw() {
    vg_clear_screen(0x00); // black background
    draw_grid();           // Tetris grid
}

PieceType random_piece_type() {
    return (PieceType)(rand() % 7);
}

void render_frame() {
    vg_clear_screen(0x00);
    draw_grid();
    draw_grid_contents(grid);
    draw_current_piece(&current_piece);
    swap_buffers();
}

void fix_piece_to_grid(const Piece* piece) {
    for (int i = 0; i < PIECE_SIZE; i++) {
        for (int j = 0; j < PIECE_SIZE; j++) {
            if (piece->shape[i][j]) {
                int gx = piece->x + j;
                int gy = piece->y + i;
                if (gx >= 0 && gx < GRID_COLS && gy >= 0 && gy < GRID_ROWS)
                    grid[gy][gx] = piece->color;
            }
        }
    }
    clear_full_lines(); // Clear lines after fixing piece to grid
}

bool move_piece(int dx, int dy) {
    int new_x = current_piece.x + dx;
    int new_y = current_piece.y + dy;

    if (movement_allowed && piece_fits(&current_piece, new_x, new_y)) {
        current_piece.x = new_x;
        current_piece.y = new_y;
        moved_this_tick = true;
        moved_in_logic_tick = true; // only counts within logic
        movement_allowed = false;
        return true;
    }

    return false;
}

void handle_keyboard_input() {
    if (scancode[0] == 0xE0) {
        switch (scancode[1]) {
            case 0x4B: // ←
                move_piece(-1, 0);
                break;
            case 0x4D: // →
                move_piece(1, 0);
                break;
            case 0x50: // ↓
                move_piece(0, 1);
                break;
        }
    }
}

int clear_full_lines() {
    int lines_cleared = 0;

    for (int row = GRID_ROWS - 1; row >= 0; row--) {
        bool full = true;
        for (int col = 0; col < GRID_COLS; col++) {
            if (grid[row][col] == 0) {
                full = false;
                break;
            }
        }

        if (full) {
            for (int r = row; r > 0; r--) {
                for (int c = 0; c < GRID_COLS; c++) {
                    grid[r][c] = grid[r - 1][c];
                }
            }

            for (int c = 0; c < GRID_COLS; c++) {
                grid[0][c] = 0;
            }

            lines_cleared++;
            row++; // Re-check this row
        }
    }

    return lines_cleared;
}

void game_tick() {
    logic_counter++;
    if (logic_counter >= LOGIC_TICK_INTERVAL) {
        logic_counter = 0;
        logic_tick();
    }

    moved_this_tick = false; // Reset for the next tick
}

void logic_tick() {
    movement_allowed = true;
    moved_in_logic_tick = false;

    fall_tick_counter++;

    // Automatic piece movement (falling)
    if (fall_tick_counter >= 60) {
        fall_tick_counter = 0;

        if (piece_fits(&current_piece, current_piece.x, current_piece.y + 1)) {
            current_piece.y++;
            is_landed = false;
            land_counter = 0;
            lock_resets = 0;
        } else {
            is_landed = true;
        }
    }

    // Lock counter
    if (is_landed) {
        if (moved_in_logic_tick) {
            if (lock_resets < MAX_LOCK_RESETS) {
                land_counter = 0;
                lock_resets++;
            }
        } else {
            land_counter++;
        }

        if (land_counter >= 30 || lock_resets >= MAX_LOCK_RESETS) {
            if (!piece_fits(&current_piece, current_piece.x, current_piece.y + 1)) {
                fix_piece_to_grid(&current_piece);
                piece_init(&current_piece, random_piece_type(), 4, 0);
                is_landed = false;
                land_counter = 0;
                lock_resets = 0;
                fall_tick_counter = 0;
            }
        }
    }
}
