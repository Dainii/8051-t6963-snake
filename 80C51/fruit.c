#include <stdlib.h>
#include "main.h"
#include "fruit.h"
#include "t6963c.h"
#include "gameboard.h"
#include "test.h"
#include "rand.h"

/**
 * Place un fruit à la position indiquée.
 * Si la position indiquée n'est pas libre, la fonction
 * cherche un endroit voisin qui soit libre.
 * @param position Position pour placer le fruit. Si nécessaire
 * le paramètre est mis à jour avec la position effective du fruit.
 */
void FRUIT_placeInFreeSpace(Position *position) {
   char c = T6963C_readFrom(position->x, position->y);

   if(c != EMPTY){
      if (T6963C_readFrom(position->x+1, position->y) == EMPTY){
	 position->x++;
      } else if (T6963C_readFrom(position->x-1, position->y) == EMPTY){
	 position->x--;
      } else if (T6963C_readFrom(position->x, position->y+1) == EMPTY){
	 position->y++;
      } else if (T6963C_readFrom(position->x, position->y-1) == EMPTY){
	 position->y--;
      } else {
	 FRUIT_random(position);
	 FRUIT_placeInFreeSpace(position);
      }
      
      T6963C_writeAt(position->x, position->y, FRUIT);
      
   } else {
      T6963C_writeAt(position->x, position->y, FRUIT); 
   }
}

/**
 * Calcule une position aléatoire pour placer un fruit.
 * La fonction ne vérifie pas si la position est libre. Par contre
 * elle ne dépasse pas le rectangle défini par les constantes 
 * SNAKE_LIMIT_X0, SNAKE_LIMIT_Y0 et SNAKE_LIMIT_X1, SNAKE_LIMIT_Y1.
 * @param position Paramètre de sortie, avec la position du fruit.
 */
void FRUIT_random(Position *position) {
   position->x = rand_interval(SNAKE_LIMIT_X0,SNAKE_LIMIT_X1);
   position->y = rand_interval(SNAKE_LIMIT_Y0,SNAKE_LIMIT_Y1);
}

/**
 * Affiche un fruit à une position aléatoire, libre, et à l'intérieur
 * du rectangle de jeu défini par 
 * SNAKE_LIMIT_X0, SNAKE_LIMIT_Y0 et SNAKE_LIMIT_X1, SNAKE_LIMIT_Y1.
 */
void FRUIT_place() {
	Position position;
	FRUIT_random(&position);
	FRUIT_placeInFreeSpace(&position);
}

#ifdef TEST
int testFruitIsPlacedInFreeSpace() {
	int testsInError = 0;
	Position position = {10, 10};
	char c;

	T6963C_writeAt(position.x, position.y, EMPTY);

	FRUIT_placeInFreeSpace(&position);
	testsInError += assertEquals(position.x, 10, "FPF1");
	testsInError += assertEquals(position.y, 10, "FPF2");
	c = T6963C_readFrom(10, 10);
	testsInError += assertEquals(c, FRUIT, "FPF3");

	FRUIT_placeInFreeSpace(&position);
	testsInError += assertNotEquals(position.x, 10, "FPF4");
	c = T6963C_readFrom(position.x, position.y);
	testsInError += assertEquals(c, FRUIT, "FPF6");

	return testsInError;
}

int testFruitIsPlacedRandomly() {
	int testsInError = 0;

	Position position1 = {50, 50};
	Position position2 = {50, 50};
	
	FRUIT_random(&position1);
	FRUIT_random(&position2);

	testsInError += assertNotEquals(position1.x, 50, "FPR1");
	testsInError += assertNotEquals(position1.y, 50, "FPR2");
	testsInError += assertNotEquals(position2.x, 50, "FPR3");
	testsInError += assertNotEquals(position2.y, 50, "FPR4");
	
	testsInError += assertNotEquals(position1.x, position2.x, "FPR5");
	testsInError += assertNotEquals(position1.y, position2.y, "FPR6");


	return testsInError;
}

int testFruit() {
	int testsInError = 0;
	
	testsInError += testFruitIsPlacedRandomly();
	testsInError += testFruitIsPlacedInFreeSpace();

	return testsInError;
}
#endif
