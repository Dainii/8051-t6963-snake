#include <mcs51reg.h>
#include <time.h>
#include "stdio-t6963c.h"
#include "test.h"
#include "buffer.h"
#include "snake.h"
#include "keyboard.h"
#include "fruit.h"
#include "gameboard.h"
#include "rand.h"

// Snake-0

#ifndef TEST

void pause(unsigned int t) {
	unsigned int n;
	for (n=0; n < t; n++);
}

void initialize() {
	STDIO_initialize();
	GMB_initialize();
}

void play() {
	Snake snake = {MOVES_RIGHT, {10, 10}, ALIVE, 5};
	unsigned char *keyboard = (unsigned char __xdata *) 0x3000;
	Arrow arrow;

	GMB_draw(SNAKE_LIMIT_X0, SNAKE_LIMIT_Y0, SNAKE_LIMIT_X1, SNAKE_LIMIT_Y1);
	FRUIT_place();
	do {
		arrow = KEYBOARD_readArrows(keyboard);
		if (SNAKE_iterate(&snake, arrow) == EATING) {
			FRUIT_place();
		}		
		pause(20000);
	} while (snake.status != DEAD);
	GMB_display(3, 7, " Le serpent est mort ");
}

void main(void) {
   
	initialize();
	play();

	while(1);
}

#else
#include <stdio.h>
void main(void) {
	int testsInError = 0;
	STDIO_initialize();

	testsInError += testBuffer();
	testsInError += testSnake();
	testsInError += testKeyboard();
	testsInError += testFruit();
	testsInError += testGameboard();
	testsInError += testRand();

	printf("%d tests en erreur", testsInError);

	while(1);
}
#endif