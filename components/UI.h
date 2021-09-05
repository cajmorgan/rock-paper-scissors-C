#include <ncurses.h>

//Game Text
char *gameMsg[2];

WINDOW *drawGameUI();
void updateGameMsg(WINDOW *gamearea, char *gameMsg);
void scoreCount(WINDOW *gamearea, struct playerStruct *playerNow);
void timerFunc(int seconds);

