#include <curses.h> 
#include <time.h>
#include <string.h>
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

void scoreCount(WINDOW *gamearea, struct playerStruct *playerNow) {
  int x, y;

  getmaxyx(gamearea, y, x);

  if(playerNow->id == 1) {
    wattron(gamearea, COLOR_PAIR(1));
    mvwprintw(gamearea, (y / 2) - 2, (x / 2) - 3, "%d", playerNow->points);
    wattroff(gamearea, COLOR_PAIR(1));
  } else if (playerNow->id == 2) {
    wattron(gamearea, COLOR_PAIR(2));
    mvwprintw(gamearea, (y / 2) - 2, (x / 2) + 1, "%d", playerNow->points);
    wattroff(gamearea, COLOR_PAIR(1));
  }
  wrefresh(gamearea);

}

void timerFunc(int seconds) {
  seconds = seconds * 400000000;
  clock_t start = clock();
  volatile unsigned sink;

  for(size_t i = 0; i < seconds; i++) {
    sink++;
  }

  clock_t end = clock();

  // double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
}