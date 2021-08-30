#include <curses.h> 
#include "UI.h"

WINDOW *drawGameUI() {
  WINDOW *gamearea;
  int stdX, stdY, startX, startY;
  getmaxyx(stdscr, stdY, stdX);

 

  

  startX = ((stdX - (stdX * 0.8)) / 2);
  startY = ((stdY - (stdY * 0.8)) / 2);
  
  gamearea = newwin(stdY * 0.8, stdX * 0.8, startY, startX);
  wborder(gamearea, ACS_DIAMOND, ACS_DIAMOND, ACS_DIAMOND, ACS_DIAMOND, ACS_DIAMOND, ACS_DIAMOND, ACS_DIAMOND, ACS_DIAMOND);
  wrefresh(gamearea);

  return gamearea;
}