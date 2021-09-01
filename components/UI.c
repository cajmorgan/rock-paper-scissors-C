#include <curses.h> 
#include "UI.h"

WINDOW *drawGameUI() {
  WINDOW *gamearea;
  int stdX, stdY, startX, startY, msgY, msgX, msgLen;
  getmaxyx(stdscr, stdY, stdX);


  startX = ((stdX - (stdX * 0.8)) / 2);
  startY = ((stdY - (stdY * 0.8)) / 2);
  
  gamearea = newwin(stdY * 0.8, stdX * 0.8, startY, startX);
  wborder(gamearea, ACS_DIAMOND, ACS_DIAMOND, ACS_DIAMOND, ACS_DIAMOND, ACS_DIAMOND, ACS_DIAMOND, ACS_DIAMOND, ACS_DIAMOND);
  wrefresh(gamearea);

  gameMsg[0] = "Loading...";
  updateGameMsg(gamearea, gameMsg[0]);
  
  return gamearea;
}

void updateGameMsg(WINDOW *gamearea, char *gameMsg) {
  int msgY, msgX, msgLen;

  getmaxyx(gamearea, msgY, msgX);
  msgLen = strlen(gameMsg);
  mvwprintw(gamearea, msgY / 2, (msgX / 2) - msgLen / 2, "%s", gameMsg);
  wrefresh(gamearea);

}
