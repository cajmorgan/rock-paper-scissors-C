#include <ncurses.h>

//Game Text
char *gameMsg[2];

WINDOW *drawGameUI();
void updateGameMsg(WINDOW *gamearea, char *gameMsg);

