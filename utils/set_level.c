#include "../board.h"

int set_level()
{
    int level = 1;

    printf("Enter a level (1-5) ");
    if (scanf("%d", &level) == 1 && level <= 5)
    {
        return level;
    }
    else if (level > 5)
    {
        return MAX_LEVEL;
    }

    return MIN_LEVEL;
}
