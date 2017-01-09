#include "test.h"
#include <string.h>

#define BUFFER_SIZE 250

/**
 * Le buffer est déclaré comme mémoire externe.
 * L'adresse de la RAM est déclarée dans l'optios --xram-loc du linker.
 */
char __xdata buffer[BUFFER_SIZE];

/** Pointeur d'entrée du buffer. */
unsigned char in = 0;

/** Pointeur de sortie du buffer. */
unsigned char out = 0;

/**
 * Insère un caractère dans le buffer.
 * @param c Le caractère.
 */
void BUFFER_in(char c) {
	buffer[in] = c;
	in ++;
}

/**
 * Récupère un caractère du buffer.
 * @return Le caractère.
 */
char BUFFER_out() {
	char c = buffer[out];
	out ++;
	return c;
}

/**
 * Réinitialise le buffer.
 */
void BUFFER_clear() {
	in = 0;
	out = 0;
}

#ifdef TEST

int testBufferOneAndThreeChar() {
	int testsInError = 0;

	BUFFER_in(1);
	testsInError += assertEquals(BUFFER_out(), 1, "BF_01");

	BUFFER_in(20);
	BUFFER_in(30);
	BUFFER_in(40);
   
	testsInError += assertEquals(BUFFER_out(), 20, "BF_02");
	testsInError += assertEquals(BUFFER_out(), 30, "BF_03");
	testsInError += assertEquals(BUFFER_out(), 40, "BF_03");

	return testsInError;
}

int testBufferClear() {
	int testsInError = 0;

	BUFFER_in(14);
	BUFFER_in(15);
	BUFFER_clear();
	BUFFER_in(16);
	testsInError += assertEquals(BUFFER_out(), 16, "BFC_01");

	return testsInError;
}

int testBufferLimit() {
	int n;

	BUFFER_clear();

	for (n = 0; n < BUFFER_SIZE; n++) {
		BUFFER_in(n % 20);
	}

	for (n = 0; n < BUFFER_SIZE; n++) {
		if (assertEquals(BUFFER_out(), n % 20, "BFL_01")) {
			return 1;
		}
	}

	return 0;
}

int testBufferLotsOfChar() {
	int n;

	for (n = BUFFER_SIZE / 2; n < BUFFER_SIZE * 2; n++) {
		BUFFER_in(n % 20);
		if (assertEquals(BUFFER_out(), n % 20, "BFL_02")) {
			return 1;
		}
	}

	return 0;
}

int testBuffer() {
	int testsInError = 0;

	testsInError += testBufferOneAndThreeChar();
	testsInError += testBufferClear();
	testsInError += testBufferLimit();
	testsInError += testBufferLotsOfChar();

	return testsInError;
}
#endif
