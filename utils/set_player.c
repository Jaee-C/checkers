#include "../board.h"

char set_player()
{
    printf("Black goes first!\n");
    printf("Which color do you want to be? (white/black) ");

    char input[6];
    while (scanf("%s", input) == 1)
    {
        for (int i = 0; i < strlen(input); i++)
        {
            input[i] = tolower(input[i]);
        }

        if (input[0] == CELL_BPIECE)
        {
            return CELL_BPIECE;
        }
        else if (input[0] == CELL_WPIECE)
        {
            return CELL_WPIECE;
        }
        else
        {
            printf("Invalid Input. Please enter 'white' or 'black'\n");
        }
    }
    return CELL_BPIECE;
}
