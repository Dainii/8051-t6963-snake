#ifndef ___GAMEBOARD_H
#define ___GAMEBOARD_H

#ifdef TEST
#define CHAR_BASE 0x00
int testGameboard();
#else
#define CHAR_BASE 0x80
#endif

#define EMPTY 0x00

#define OBSTACLE_A (0x21 + CHAR_BASE)
#define OBSTACLE_B (0x22 + CHAR_BASE)
#define OBSTACLE_C (0x23 + CHAR_BASE)
#define OBSTACLE_D (0x24 + CHAR_BASE)
#define OBSTACLE_E (0x25 + CHAR_BASE)
#define OBSTACLE_F (0x26 + CHAR_BASE)
#define OBSTACLE_G (0x27 + CHAR_BASE)
#define OBSTACLE_H (0x28 + CHAR_BASE)

#define SNAKE_BODY (0x11 + CHAR_BASE)
#define SNAKE_SWALLOW (0x12 + CHAR_BASE)
#define SNAKE_HEAD (0x13 + CHAR_BASE)
#define SNAKE_DEAD (0x14 + CHAR_BASE)

#define FRUIT (0x15 + CHAR_BASE)

void GMB_initialize();
void GMB_draw(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1);
void GMB_clear(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1);
void GMB_display(unsigned char x0, unsigned char y0, char *text);

#endif
