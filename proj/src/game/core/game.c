#include "include/game.h"
#include "../ui/include/game_ui.h"
#include "../states/include/state.h"

void game_logic_init(GameLogic* game) {
    if (!game) return;

    game->drop_timer = 0;
    game->current_drop_speed = DROP_SPEED_INITIAL;
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
    if (game->drop_timer >= game->current_drop_speed) {
        game->drop_timer = 0;
        game_logic_drop_piece(game);
        needs_redraw = true;
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
            GameScore* score = tetris_get_score();
            scoreAddLines(score, lines);
            game_logic_update_speed(game);
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

    bool moved = false;

    switch (event.action) {
        case INPUT_LEFT:
            moved = game_logic_move_piece(game, -1, 0);
            break;
        case INPUT_RIGHT:
            moved = game_logic_move_piece(game, 1, 0);
            break;
        case INPUT_DOWN:
            moved = game_logic_move_piece(game, 0, 1);
            break;
        case INPUT_ROTATE:
            game_logic_rotate_piece(game);
            moved = true;
            break;
        case INPUT_DROP:
            // Drop rápido
            while (game_logic_move_piece(game, 0, 1)) {
                // Continue dropping
            }
            game_logic_drop_piece(game);
            moved = true;
            break;
        default:
            break;
    }
    if (moved) {
        needs_redraw = true;
    }
}

void game_logic_render(const GameLogic* game) {
    if (!game) return;
    // Limpar ecrã
    vg_clear_screen(COLOR_BACKGROUND);

    draw_moldure();
    draw_grid_background();

    // Desenhar conteúdo da grid
    extern int grid[GRID_ROWS][GRID_COLS];
    draw_grid_contents(grid);
    draw_score_info();
    draw_highscores();

    // Desenhar peça atual
    if (!game->game_over) {
        draw_current_piece(&game->current_piece);
    }
    draw_grid();
}

void game_logic_update_speed(GameLogic* game) {
    GameScore* score = tetris_get_score();

    // Reduz 1 frames por cada linha completada
    int speed_reduction = (score->level-1)*5;
    game->current_drop_speed = DROP_SPEED_INITIAL - speed_reduction;

    // Limite mínimo para manter jogabilidade - depois podemos mudar isto conforme o nivel
    if (game->current_drop_speed < DROP_SPEED_FAST) {
        game->current_drop_speed = DROP_SPEED_FAST;
    }

    // Limite máximo (velocidade mais lenta)
    if (game->current_drop_speed > DROP_SPEED_INITIAL) {
        game->current_drop_speed = DROP_SPEED_INITIAL;
    }
}
