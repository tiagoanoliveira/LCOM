//
// Created by tiago-oliveira on 27-05-2025.
//

#include "game.h"

static int dropTimer = 0;

void game_init(void) {
    tetris_init();
    dropTimer = 0;
}

void game_cleanup(void) {
    // Cleanup se necessário
}

void game_handle_input(uint8_t scancode[2], bool two_byte) {
    // ESC - voltar ao menu
    if (scancode[0] == 0x81) {
        state_set_current(STATE_MENU);
        state_set_redraw(true);
        return;
    }

    if (two_byte && scancode[0] == 0xE0) {
        switch (scancode[1]) {
            case 0x4B: // Seta esquerda
                game_move_piece(-1, 0);
                break;
            case 0x4D: // Seta direita
                game_move_piece(1, 0);
                break;
            case 0x50: // Seta baixo
                game_move_piece(0, 1);
                break;
        }
    }

    // Rotação com espaço
    if (scancode[0] == 0x39) { // Espaço
        game_rotate_piece(true);
    }
}

void game_update(void) {
    dropTimer++;
    if (dropTimer >= DROP_SPEED_INITIAL) {
        dropTimer = 0;
        game_drop_piece();
    }
}

bool game_move_piece(int deltaX, int deltaY) {
    Piece* piece = state_get_current_piece();
    if (piece_fits(piece, piece->x + deltaX, piece->y + deltaY)) {
        piece->x += deltaX;
        piece->y += deltaY;
        state_set_redraw(true);
        return true;
    }
    return false;
}

void game_rotate_piece(bool clockwise) {
    Piece* piece = state_get_current_piece();
    int oldRotation = piece->rotation;

    piece_rotate(piece, clockwise);

    if (!piece_fits(piece, piece->x, piece->y)) {
        // Reverter rotação se não couber
        piece->rotation = oldRotation;
        piece_update_shape(piece);
    } else {
        state_set_redraw(true);
    }
}

void game_drop_piece(void) {
    if (!game_move_piece(0, 1)) {
        // Peça não pode descer mais
        Piece* piece = state_get_current_piece();
        fix_piece_to_grid(piece);
        clear_full_lines();
        game_spawn_new_piece();
    }
}

void game_spawn_new_piece(void) {
    Piece* piece = state_get_current_piece();
    piece_init(piece, random_piece_type(), 3, 0);

    if (!piece_fits(piece, piece->x, piece->y)) {
        state_set_game_over(true);
    } else {
        state_set_redraw(true);
    }
}

