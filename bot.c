#include "board.h"

void bot_move(board_t board, int* count, char* player, int level) {
    // Computer's move
    system("clear");
    perform_next_action(board, *player, *count, level);
    *player = change_player(*player);   // After every move, change player
}
