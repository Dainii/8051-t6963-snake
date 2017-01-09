#include "test.h"
#include "bdd.h"
#include "t6963c.h"
#include "buffer.h"
#include "gameboard.h"
#include "snake.h"

#define FRUIT_CALORIES 3

/**
 * Modifie les coordonnï¿½es du serpent selon sa direction.
 * @param snake La description du serpent.
 */
void SNAKE_move(Snake *snake) {
	// Garde en mémoire l'ancienne position
	BUFFER_in(snake->position.x);
	BUFFER_in(snake->position.y);

	// Modifie en fonction de la direction du snake
	switch (snake->direction) {
		case MOVES_UP :
			snake->position.y--;
			break;

		case MOVES_DOWN :
			snake->position.y++;
			break;

		case MOVES_LEFT :
			snake->position.x--;
			break;

		case MOVES_RIGHT :
			snake->position.x++;
			break;

		default: 
			break;
	}
}

/**
 * Dï¿½cide si le serpent vit ou meurt, ou mange un fruit, selon
 * sa position et ce qui se trouve ï¿½ cet endroit.
 * @param snake La description du serpent.
 */
void SNAKE_liveOrDie(Snake *snake) {
	unsigned char c = T6963C_readFrom(snake->position.x, snake->position.y);

	switch (c) {
		case OBSTACLE_A:
		case OBSTACLE_B:
		case OBSTACLE_C:
		case OBSTACLE_D:
		case OBSTACLE_E:
		case OBSTACLE_F:
		case OBSTACLE_G:
		case OBSTACLE_H:
		case SNAKE_BODY:  
			snake->status = DEAD;
			break;

		case FRUIT :
			snake->status = EATING;
			snake->caloriesLeft += FRUIT_CALORIES;
			break;

		default: 
			snake->status = ALIVE;
	}
}

/**
 * Affiche la tï¿½te du serpent.
 * @param snake La dï¿½finition du serpent.
 */
void SNAKE_showHead(Snake *snake) {
	if (snake->status == DEAD){
		T6963C_writeAt(snake->position.x, snake->position.y, SNAKE_DEAD);
	} 
	if (snake->status == ALIVE){
		T6963C_writeAt(snake->position.x, snake->position.y, SNAKE_HEAD);
	}
}

/**
 * Affiche le corps du serpent, et efface la queue du serpent.
 * Aussi, si le serpent a mangï¿½ un fruit, affiche un caractï¿½re spï¿½cial.
 * @param snake La dï¿½finition du serpent.
 */
void SNAKE_showBody(Snake *snake) {
	// On commence par remplacer la tête par le corps
	 if (snake->status == EATING){
	    T6963C_writeAt(snake->position.x, snake->position.y, SNAKE_SWALLOW);
	 } else {
	    T6963C_writeAt(snake->position.x, snake->position.y, SNAKE_BODY);
	 }
	
	// Efface la queue
	if (snake->caloriesLeft == 0){
		T6963C_writeAt(BUFFER_out(), BUFFER_out(), EMPTY);
	}	
	
	if (snake->caloriesLeft != 0){
	    snake->caloriesLeft --;
	}
}

/**
 * Dï¿½cide de varier la direction du serpent selon la direction indiquï¿½e.
 * Le serpent ne peut jamais tourner de 180ï¿½ en un mouvement.
 * @param snake La description du serpent.
 * @param arrow La direction dï¿½sirï¿½e.
 */
void SNAKE_turn(Snake *snake, Arrow arrow) {
	switch (arrow) {
		case ARROW_RIGHT :
			if (snake->direction != MOVES_LEFT){
				snake->direction = MOVES_RIGHT;
			}
			break;

		case ARROW_LEFT :
			if (snake->direction != MOVES_RIGHT){
				snake->direction = MOVES_LEFT;
			}
			break;

		case ARROW_UP :
			if (snake->direction != MOVES_DOWN){
				snake->direction = MOVES_UP;
			}
			break;

		case ARROW_DOWN :
			if (snake->direction != MOVES_UP){
				snake->direction = MOVES_DOWN;
			}
			break;

		default: 
			break;
	}
}

/**
 * Effectue une itï¿½ration dans la vie du serpent.
 * @param snake La dï¿½finition du serpent.
 * @return L'ï¿½tat du serpent aprï¿½s l'itï¿½ration.
 */
Status SNAKE_iterate(Snake *snake, Arrow arrow) {
	SNAKE_showBody(snake);
	SNAKE_turn(snake, arrow);
	SNAKE_move(snake);
	SNAKE_liveOrDie(snake);
	SNAKE_showHead(snake);

	return snake->status;
}

#ifdef TEST

// ========================== Tests unitaires =================================
// Chaque test vï¿½rifie le comportement d'une fonctionnalitï¿½ en ï¿½tablissant
// un ï¿½tat initial et en vï¿½rifiant l'ï¿½tat final.
int testSnakeTurnsTo(Direction currentDirection, Arrow turn, Direction expectedResult, char *testCode) {
	Snake snake = {MOVES_RIGHT, {10, 10}, ALIVE, 0};
	snake.direction = currentDirection;
	SNAKE_turn(&snake, turn);
	return assertEquals(expectedResult, snake.direction, testCode);	
}

int testSnakeTurns() {
	int testsInError = 0;

	testsInError += testSnakeTurnsTo(MOVES_RIGHT, ARROW_RIGHT,	MOVES_RIGHT,"ST01");
	testsInError += testSnakeTurnsTo(MOVES_RIGHT, ARROW_LEFT,	MOVES_RIGHT,"ST02");
	testsInError += testSnakeTurnsTo(MOVES_RIGHT, ARROW_UP,		MOVES_UP, 	"ST03");
	testsInError += testSnakeTurnsTo(MOVES_RIGHT, ARROW_DOWN, 	MOVES_DOWN, "ST04");

	testsInError += testSnakeTurnsTo(MOVES_LEFT, ARROW_RIGHT, 	MOVES_LEFT, "ST11");
	testsInError += testSnakeTurnsTo(MOVES_LEFT, ARROW_LEFT, 	MOVES_LEFT, "ST12");
	testsInError += testSnakeTurnsTo(MOVES_LEFT, ARROW_UP, 		MOVES_UP, 	"ST13");
	testsInError += testSnakeTurnsTo(MOVES_LEFT, ARROW_DOWN, 	MOVES_DOWN, "ST14");

	testsInError += testSnakeTurnsTo(MOVES_UP, ARROW_RIGHT, 	MOVES_RIGHT,"ST21");
	testsInError += testSnakeTurnsTo(MOVES_UP, ARROW_LEFT, 		MOVES_LEFT, "ST22");
	testsInError += testSnakeTurnsTo(MOVES_UP, ARROW_UP, 		MOVES_UP, 	"ST23");
	testsInError += testSnakeTurnsTo(MOVES_UP, ARROW_DOWN, 		MOVES_UP, 	"ST24");

	testsInError += testSnakeTurnsTo(MOVES_DOWN, ARROW_RIGHT, 	MOVES_RIGHT,"ST31");
	testsInError += testSnakeTurnsTo(MOVES_DOWN, ARROW_LEFT, 	MOVES_LEFT, "ST32");
	testsInError += testSnakeTurnsTo(MOVES_DOWN, ARROW_UP, 		MOVES_DOWN, "ST33");
	testsInError += testSnakeTurnsTo(MOVES_DOWN, ARROW_DOWN, 	MOVES_DOWN, "ST34");

	return testsInError;
}

int testSnakeEatsAFruit() {
	int testsInError = 0;
	Snake snake = {MOVES_RIGHT, {BDD_SCREEN_X, BDD_SCREEN_Y}, ALIVE, 0};
	
	T6963C_writeAt(BDD_SCREEN_X, BDD_SCREEN_Y, FRUIT);
	SNAKE_liveOrDie(&snake);

	testsInError += assertEquals(snake.status, EATING, "SF001");
	testsInError += assertEquals(snake.caloriesLeft, FRUIT_CALORIES, "SF002");

	T6963C_writeAt(BDD_SCREEN_X, BDD_SCREEN_Y, EMPTY);
	SNAKE_liveOrDie(&snake);
	testsInError += assertEquals(snake.status, ALIVE, "SF003");

	return testsInError;	
}

int testSnakeMoves() {
	int testsInError = 0;
	Snake snake;

	snake.position.x = 10;
	snake.position.y = 10;

	snake.direction = MOVES_UP;
	SNAKE_move(&snake);
	testsInError += assertEquals(9, snake.position.y, "SM001");

	snake.direction = MOVES_DOWN;
	SNAKE_move(&snake);
	testsInError += assertEquals(10, snake.position.y, "SM002");

	snake.direction = MOVES_LEFT;
	SNAKE_move(&snake);
	testsInError += assertEquals( 9, snake.position.x, "SM003");

	snake.direction = MOVES_RIGHT;
	SNAKE_move(&snake);
	testsInError += assertEquals(10, snake.position.x, "SM004");

	return testsInError;
}

int testSnakeHitsABorder() {
	int testsInError = 0;

	Snake snake;

	snake.status = ALIVE;
	snake.position.x = SNAKE_LIMIT_X0 + 1;
	snake.position.y = SNAKE_LIMIT_Y0 + 1;
	SNAKE_liveOrDie(&snake);
	testsInError += assertEquals(snake.status, ALIVE, "SO01");

	snake.status = ALIVE;
	snake.position.x = SNAKE_LIMIT_X0;
	snake.position.y = SNAKE_LIMIT_Y0 + 1;
	SNAKE_liveOrDie(&snake);
	testsInError += assertEquals(snake.status, DEAD, "SO02");

	snake.status = ALIVE;
	snake.position.x = SNAKE_LIMIT_X0 + 1;
	snake.position.y = SNAKE_LIMIT_Y0;
	SNAKE_liveOrDie(&snake);
	testsInError += assertEquals(snake.status, DEAD, "SO03");

	snake.status = ALIVE;
	snake.position.x = SNAKE_LIMIT_X1;
	snake.position.y = SNAKE_LIMIT_Y1 - 1;
	SNAKE_liveOrDie(&snake);
	testsInError += assertEquals(snake.status, DEAD, "SO04");

	snake.status = ALIVE;
	snake.position.x = SNAKE_LIMIT_X1 - 1;
	snake.position.y = SNAKE_LIMIT_Y1;
	SNAKE_liveOrDie(&snake);
	testsInError += assertEquals(snake.status, DEAD, "SO05");

	return testsInError;
}

// =========================== Tests de comportement ============================
// Chaque test:
// 1- ï¿½tablit un ï¿½tat initial.
// 2- Simule un scï¿½nario qu'un utilisateur pourrait rï¿½aliser manuellement.
// 3- Vï¿½rifie, en contrï¿½lant le contenu de l'ï¿½cran, que ce 
//    que percevrait l'utilisateur est juste

int bddSnakeHitsItsTail() {
	BddExpectedContent c = {
		" 14111....",
		"..1..1....",
		"..1111....",
		"..........",
		".........."
	};
	Snake snake = {MOVES_RIGHT, {BDD_SCREEN_X, BDD_SCREEN_Y}, ALIVE, 11};
	char n;

	BUFFER_clear();
	BDD_clear();
	
	for (n = 0; n < 5; n++) {
		SNAKE_iterate(&snake, ARROW_NEUTRAL);
	}
	for (n = 0; n < 2; n++) {
		SNAKE_iterate(&snake, ARROW_DOWN);
	}
	for (n = 0; n < 3; n++) {
		SNAKE_iterate(&snake, ARROW_LEFT);
	}
	for (n = 0; n < 2; n++) {
		SNAKE_iterate(&snake, ARROW_UP);
	}

	return BDD_assert(c, "SNT");
}


int bddSnakeHitsThisObstacle(char obstacle, char *testId) {
	BddExpectedContent c = {
		"  1114....",
		"..........",
		"..........",
		"..........",
		".........."
	};
	Snake snake = {MOVES_RIGHT, {BDD_SCREEN_X, BDD_SCREEN_Y}, ALIVE, 3};
	char n;

	BUFFER_clear();
	BDD_clear();
	T6963C_writeAt(BDD_SCREEN_X + 5, BDD_SCREEN_Y, obstacle);
	
	for (n = 0; n < 5; n++) {
		SNAKE_iterate(&snake, ARROW_NEUTRAL);
	}

	return BDD_assert(c, testId);
}

int bddSnakeHitsAnObstacle() {
	int testsInError = 0;

	testsInError += bddSnakeHitsThisObstacle(OBSTACLE_A, "SNO-A");
	testsInError += bddSnakeHitsThisObstacle(OBSTACLE_B, "SNO-B");
	testsInError += bddSnakeHitsThisObstacle(OBSTACLE_C, "SNO-C");
	testsInError += bddSnakeHitsThisObstacle(OBSTACLE_D, "SNO-D");
	testsInError += bddSnakeHitsThisObstacle(OBSTACLE_E, "SNO-E");
	testsInError += bddSnakeHitsThisObstacle(OBSTACLE_F, "SNO-F");
	testsInError += bddSnakeHitsThisObstacle(OBSTACLE_G, "SNO-G");
	testsInError += bddSnakeHitsThisObstacle(OBSTACLE_H, "SNO-H");

	return testsInError;
}

int bddSnakeMovesTurnsAndCatchesAFruit() {
	BddExpectedContent c = {
		"      1...",
		"......2...",
		"......1...",
		"...3111...",
		".........."
	};
	Snake snake = {MOVES_RIGHT, {BDD_SCREEN_X, BDD_SCREEN_Y}, ALIVE, 3};
	char n;

	BUFFER_clear();
	BDD_clear();
	T6963C_writeAt(BDD_SCREEN_X + 6, BDD_SCREEN_Y + 1, FRUIT);
	
	for (n = 0; n < 6; n++) {
		SNAKE_iterate(&snake, ARROW_NEUTRAL);
	}
	for (n = 0; n < 3; n++) {
		SNAKE_iterate(&snake, ARROW_DOWN);
	}
	for (n = 0; n < 3; n++) {
		SNAKE_iterate(&snake, ARROW_LEFT);
	}

	return BDD_assert(c, "SNTF");
}

int bddSnakeGrows() {
	BddExpectedContent c = {
		"    1113..",
		"..........",
		"..........",
		"..........",
		".........."
	};
	Snake snake = {MOVES_RIGHT, {BDD_SCREEN_X, BDD_SCREEN_Y}, ALIVE, 3};
	char n;

	BUFFER_clear();
	BDD_clear();
	for (n=0; n<7; n++) {
		SNAKE_iterate(&snake, ARROW_NEUTRAL);
	}
	
	return BDD_assert(c, "SNG");
}

/**
 * Collection de tests.
 * Les tests en erreur sont affichï¿½s ï¿½ l'ï¿½cran.
 * @return Le nombre de tests ï¿½chouï¿½s.
 */
int testSnake() {
	int testsInError = 0;

	// Tests unitaires:
	testsInError += testSnakeTurns();
	testsInError += testSnakeMoves();
	// testsInError += testSnakeHitsABorder();
	testsInError += testSnakeEatsAFruit();

	// Tests de comportement:
	testsInError += bddSnakeGrows();
	testsInError += bddSnakeMovesTurnsAndCatchesAFruit();
	testsInError += bddSnakeHitsAnObstacle();
	testsInError += bddSnakeHitsItsTail();

	// Nombre de tests en erreur:
	return testsInError;
}

#endif
