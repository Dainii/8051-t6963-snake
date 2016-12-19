#include <stdlib.h>
#include <time.h>
#include "main.h"
#include "fruit.h"
#include "t6963c.h"
#include "gameboard.h"
#include "test.h"


/**
 *
 * Permet de retourner un nombre aléatoire entre deux bornes
 *
 */
unsigned char rand_interval(int min, int max)
{
    unsigned char r;

    do
    {
        r = rand();
    } while (r < min || r > max );

    return r;
}
 
