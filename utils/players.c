#include "../board.h"

int get_player_num(void)
{
    printf("Enter number of players (1 or 2): ");

    int n;
    scanf("%d", &n);

    if (n < 1 || n > 2)
    {
        printf("Enter 1 or 2.\n");
        exit(EXIT_FAILURE);
    }

    return n;
}
