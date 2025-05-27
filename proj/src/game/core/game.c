#include "game.h"
#include "../ui/render.h"
#include "../../drivers/graphics/graphics.h"
#include "tetris.h"

void game_logic_init(GameLogic* game) {
    if (!game) return;

    game->score = 0;
    game->lines_cleared = 0;
    game->level = 1;
    game->drop_timer = 0;
    game->game_over = false;

    // Inicializar Tetris
    tetris_init();

    // Spawn primeira peça
    game_logic_spawn_piece(game);
}

void game_logic_update(GameLogic* game) {
    if (!game || game->game_over) return;

    // Timer de queda automática
    game->drop_timer++;
    if (game->drop_timer >= DROP_SPEED_INITIAL) {
        game->drop_timer = 0;
        game_logic_drop_piece(game);
    }
}

bool game_logic_move_piece(GameLogic* game, int deltaX, int deltaY) {
    if (!game || game->game_over) return false;

    Piece* piece = &game->current_piece;

    if (piece_fits(piece, piece->x + deltaX, piece->y + deltaY)) {
        piece->x += deltaX;
        piece->y += deltaY;
        return true;
    }

    return false;
}

void game_logic_rotate_piece(GameLogic* game) {
    if (!game || game->game_over) return;

    Piece* piece = &game->current_piece;
    int old_rotation = piece->rotation;

    piece_rotate(piece, true);

    if (!piece_fits(piece, piece->x, piece->y)) {
        // Reverter rotação se não couber
        piece->rotation = old_rotation;
        piece_update_shape(piece);
    }
}

void game_logic_drop_piece(GameLogic* game) {
    if (!game || game->game_over) return;

    if (!game_logic_move_piece(game, 0, 1)) {
        // Peça não pode descer mais
        game_logic_fix_piece(game);

        int lines = game_logic_clear_lines(game);
        if (lines > 0) {
            game->lines_cleared += lines;
            game->score += lines * 100 * game->level;
            game->level = 1 + game->lines_cleared / 10;
        }

        game_logic_spawn_piece(game);
    }
}

void game_logic_fix_piece(GameLogic* game) {
    if (!game) return;

    fix_piece_to_grid(&game->current_piece);
}

int game_logic_clear_lines(GameLogic* game) {
    if (!game) return 0;

    return clear_full_lines();
}

void game_logic_spawn_piece(GameLogic* game) {
    if (!game) return;

    piece_init(&game->current_piece, random_piece_type(), 3, 0);

    if (!piece_fits(&game->current_piece, game->current_piece.x, game->current_piece.y)) {
        game->game_over = true;
    }
}

bool game_logic_is_game_over(const GameLogic* game) {
    return game ? game->game_over : true;
}

void game_logic_handle_input(GameLogic* game, InputEvent event) {
    if (!game || game->game_over || !event.pressed) return;

    switch (event.action) {
        case INPUT_LEFT:
            game_logic_move_piece(game, -1, 0);
            break;
        case INPUT_RIGHT:
            game_logic_move_piece(game, 1, 0);
            break;
        case INPUT_DOWN:
            game_logic_move_piece(game, 0, 1);
            break;
        case INPUT_ROTATE:
            game_logic_rotate_piece(game);
            break;
        case INPUT_DROP:
            // Drop rápido
            while (game_logic_move_piece(game, 0, 1)) {
                // Continue dropping
            }
            game_logic_drop_piece(game);
            break;
        default:
            break;
    }
}

void game_logic_render(const GameLogic* game) {
    if (!game) return;

    vg_clear_screen(BACKGROUND_COLOR);

    // Desenhar grid
    draw_grid();

    // Desenhar conteúdo da grid
    extern int grid[GRID_ROWS][GRID_COLS];
    draw_grid_contents(grid);

    // Desenhar peça atual
    if (!game->game_over) {
        draw_current_piece(&game->current_piece);
    }

    // Desenhar informações do jogo (score, level, etc.)
    // Isto pode ser movido para uma função UI separada
}
