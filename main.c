#include <curses.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "main.h"
#include "./components/UI.c"



//Gör sedan en algorithm där datorn får en högre procent chans att välja ett visst föremål beroende på var player väljer under tid

/* Functions */

int main() {
  //Initiate random numbers
  srand(time(NULL));

  WINDOW *gamearea;
  WINDOW *weaponsWinOne;
  WINDOW *weaponsWinTwo;

  //Initializing params
  initscr();
  cbreak();
  noecho();
  start_color();
  init_pair(10, COLOR_WHITE, COLOR_BLACK);
  bkgd(COLOR_PAIR(10));
  curs_set(0);
  wborder(stdscr, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  refresh();

  
  gamearea = drawGameUI();
  
  playerOne = (struct playerStruct *)malloc(sizeof(struct playerStruct));
  playerTwo = (struct playerStruct *)malloc(sizeof(struct playerStruct));

  createPlayers(1, gamearea, "PlayerOne", playerOne);
  createPlayers(2, gamearea, "PlayerTwo", playerTwo);

  weaponsWinOne = createWeapons(playerOne, gamearea);
  weaponsWinTwo = createWeapons(playerTwo, gamearea);
  
  //Initiate weapons on screen
  weaponSelector(weaponsWinOne, playerOne, playerOne->weapon);
  weaponSelector(weaponsWinTwo, playerTwo, playerTwo->weapon);

  gameLoop(weaponsWinOne, weaponsWinTwo);
  // printw("%s",playerOne->weapons[1]);

  getch();

  endwin();

  return 0;
}

void createPlayers(int id, WINDOW *gamearea, char *playerName, struct playerStruct *playerNow) {
  playerNow->id = id;
  playerNow->points = 0;
  playerNow->name = playerName;
  playerNow->turn = false;
  playerNow->ai = false;
  playerNow->weapon = 0;

  int gWinX, gWinY;
  getmaxyx(gamearea, gWinY, gWinX);
  wattrset(gamearea, A_BOLD);
  start_color();

  init_pair(1, COLOR_WHITE, COLOR_GREEN);
  init_pair(2, COLOR_WHITE, COLOR_RED);

  if(id == 1) {
    wattron(gamearea, COLOR_PAIR(1));
    mvwprintw(gamearea, gWinY - 1, (gWinX / 2) - (strlen(playerName) / 2), playerName);
    wattroff(gamearea, COLOR_PAIR(1));
  } else if(id == 2) {
    wattron(gamearea, COLOR_PAIR(2));
    mvwprintw(gamearea, 0, (gWinX / 2) - (strlen(playerName) / 2), playerName);
    wattroff(gamearea, COLOR_PAIR(2));
  }

  wrefresh(gamearea);
}


WINDOW *createWeapons(struct playerStruct *playerNow, WINDOW *gamearea) {
  WINDOW *weaponsWin;
  int startX, startY, width, height;

  getmaxyx(gamearea, startY, startX);
  height = startY * 0.25;
  width = startX * 0.5;

  static char *weapons[] = {
    "Rock", "Paper", "Scissors"
  };

  playerNow->weapons = (char **)malloc(sizeof(char **) * 3);

  playerNow->weapons = weapons;

  if(playerNow->id == 1) {
    wattron(gamearea, COLOR_PAIR(1));
    weaponsWin = derwin(gamearea, height, width, (startY - height) - 3, (startX / 2) - width / 2);
    wattroff(gamearea, COLOR_PAIR(1));
  } else {
    wattron(gamearea, COLOR_PAIR(2));
    weaponsWin = derwin(gamearea, height, width, 3, (startX / 2) - width / 2);
    wattroff(gamearea, COLOR_PAIR(2));
  }

  wborder(weaponsWin, '*', '*', '*', '*', '*', '*', '*', '*');
  wrefresh(weaponsWin);

  return weaponsWin;
}


void weaponSelector(WINDOW *weaponsWin, struct playerStruct *playerNow, int selectedWeapon) {
  int placeX, placeY, height, width;
  placeX = 4; 

  wattrset(weaponsWin, A_NORMAL);
  getmaxyx(weaponsWin, height, width);
  
  for(int i = 0; i < 3; i++) {
    int len = strlen(playerNow->weapons[i]);
    if(i == selectedWeapon && playerNow->turn == true) {
      wattrset(weaponsWin, A_BLINK);
      mvwprintw(weaponsWin, 3, placeX, "%s", playerNow->weapons[i]);
      wattroff(weaponsWin, A_BLINK);
    } else {
      mvwprintw(weaponsWin, 3, placeX, "%s", playerNow->weapons[i]);
    }

    wrefresh(weaponsWin);
    placeX += (width / 3) - len / 2;
  }
}

void gameLoop(WINDOW *weaponsWinOne, WINDOW *weaponsWinTwo) {
  int whoStarts, ch;
  whoStarts = 0;

  while(whoStarts != 1 && whoStarts != 2) {
    whoStarts = rand() % 10;
    if(whoStarts == 1) {
      playerOne->turn = true;
      keypad(weaponsWinOne, TRUE);
    } else if(whoStarts == 2) {
      playerTwo->turn = true;
      keypad(weaponsWinTwo, TRUE);
    }
  }

  weaponSelector(weaponsWinOne, playerOne, playerOne->weapon);
  weaponSelector(weaponsWinTwo, playerTwo, playerTwo->weapon);

  while(1) {
    if(playerOne->turn == true && playerOne->ai == false) {
      ch = wgetch(weaponsWinOne);
      switch(ch) {
        case KEY_LEFT: 
          if(playerOne->weapon == 0) {
            playerOne->weapon = 2;
            weaponSelector(weaponsWinOne, playerOne, playerOne->weapon);
          }
          else {
            playerOne->weapon--;
            weaponSelector(weaponsWinOne, playerOne, playerOne->weapon);
          }
          break;
        case KEY_RIGHT:
          if(playerOne->weapon == 2) {
            playerOne->weapon = 0;
            weaponSelector(weaponsWinOne, playerOne, playerOne->weapon);
          }
          else {
            playerOne->weapon++;
            weaponSelector(weaponsWinOne, playerOne, playerOne->weapon);
          }
          break;
      }
    } else if (playerTwo->turn == true && playerTwo->ai == false) {
      ch = wgetch(weaponsWinTwo);
      switch(ch) {
        case KEY_LEFT: 
          if(playerTwo->weapon == 0) {
            playerTwo->weapon = 2;
            weaponSelector(weaponsWinTwo, playerTwo, playerTwo->weapon);
          }
          else {
            playerTwo->weapon--;
            weaponSelector(weaponsWinTwo, playerTwo, playerTwo->weapon);
          }
          break;
        case KEY_RIGHT:
          if(playerTwo->weapon == 2) {
            playerTwo->weapon = 0;
            weaponSelector(weaponsWinTwo, playerTwo, playerTwo->weapon);
          }
          else {
            playerTwo->weapon++;
            weaponSelector(weaponsWinTwo, playerTwo, playerTwo->weapon);
          }
          break;
      }
    }
  }
}