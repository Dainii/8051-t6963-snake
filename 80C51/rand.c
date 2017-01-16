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
 * @param min, max Définit les bornes minimum et maximum du nombre aléatoire
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
 


#ifdef TEST
int testTrueRandom() {
	int testsInError = 0;
	char c, d;

	c = rand_interval(1, 100);
	d = rand_interval(1, 100);
	testsInError += assertNotEquals(c, d, "RTR1");

	return testsInError;
}

int testRand() {
	int testsInError = 0;
	
	testsInError += testTrueRandom();

	return testsInError;
}
#endif