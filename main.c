#include <curses.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


//Gör sedan en algorithm där datorn får en högre procent chans att välja ett visst föremål beroende på var player väljer

/* global params */
struct playerStruct {
    int id;
    int points;
    char *name;
    bool turn;
    int weapon;
    char **weapons;
  } *playerOne, *playerTwo;


/* Function Declarations */
WINDOW *drawGameUI();
void createPlayers(int id, WINDOW *gamearea, char *playerName, struct playerStruct *playerNow);
WINDOW *createWeapons(struct playerStruct *playerNow, WINDOW *gamearea);
void weaponSelector(WINDOW *weaponsWin, struct playerStruct *playerNow);

/* Functions */

int main() {
  WINDOW *gamearea;
  WINDOW *weaponsWinYou;
  WINDOW *weaponsWinComputer;

  initscr();

  cbreak();
  noecho();

  wborder(stdscr, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  refresh();
  gamearea = drawGameUI();

  
  playerOne = (struct playerStruct *)malloc(sizeof(struct playerStruct));
  playerTwo = (struct playerStruct *)malloc(sizeof(struct playerStruct));

  createPlayers(1, gamearea, "You", playerOne);
  createPlayers(2, gamearea, "Computer", playerTwo);

  weaponsWinYou = createWeapons(playerOne, gamearea);
  weaponsWinComputer = createWeapons(playerTwo, gamearea);
  
  //Initiate weapons on screen
  weaponSelector(weaponsWinYou, playerOne);
  weaponSelector(weaponsWinComputer, playerTwo);

  // printw("%s",playerOne->weapons[1]);

  getch();

  endwin();

  return 0;
}

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

void createPlayers(int id, WINDOW *gamearea, char *playerName, struct playerStruct *playerNow) {
  playerNow->id = id;
  playerNow->points = 0;
  playerNow->name = playerName;
  playerNow->turn = false;
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
  height = startY * 0.2;
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
    wattroff(gamearea, COLOR_PAIR(1));
  }

  wborder(weaponsWin, '*', '*', '*', '*', '*', '*', '*', '*');
  wrefresh(weaponsWin);

  return weaponsWin;
}


void weaponSelector(WINDOW *weaponsWin, struct playerStruct *playerNow) {
  int placeX, placeY, height, width;
  placeX = 4;

  getmaxyx(weaponsWin, height, width);

  for(int i = 0; i < 3; i++) {
    int len = strlen(playerNow->weapons[i]);
    mvwprintw(weaponsWin, 2, placeX, "%s", playerNow->weapons[i]);
    wrefresh(weaponsWin);
    placeX += (width / 3) - len / 2;
  }
}