#include <curses.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h> 
#include <time.h>
#include "main.h"
#include "./components/UI.c"
#include "./components/AI.c"

/* Functions */
int main() {

  //Initiate random numbers
  srand(time(NULL));

  WINDOW *gamearea;
  WINDOW *weaponsWinOne;
  WINDOW *weaponsWinTwo;

  char *playerNameOne, *playerNameTwo; 
  char howManyPlayers;
  int howManyRounds;
  int enteredWithErr;
  bool isRobot = false;

  playerNameOne = (char *)malloc(sizeof(char) * 100);
  playerNameTwo = (char *)malloc(sizeof(char) * 100);  
  
  printf("\033[1;31m");
  puts("How many players? Enter '1' or '2'");
  enteredWithErr = 1;
  while(1) {
    scanf("%c", &howManyPlayers); 
    if(howManyPlayers == '2') {
      printf("\033[1;35m");
      puts("Enter name of Player One... (no spaces)");
      scanf("%s", playerNameOne);
      printf("\033[1;34m");
      puts("Enter name of Player Two... (no spaces)");
      scanf("%s", playerNameTwo);
      break;
    } else if (howManyPlayers == '1') {
      printf("\033[1;35m");
      puts("Enter name of Player One... (no spaces)");
      scanf("%s", playerNameOne);
      playerNameTwo = "AI";
      isRobot = true;
      break;
    } else if (enteredWithErr == 1) {
      puts("Only '1' or '2'!!!");
      scanf("%c", &howManyPlayers); 
      enteredWithErr += 1;
    }
  }

  enteredWithErr = 1;
  printf("\033[1;31m");
  puts("How many rounds?");
  while(1) {    
    howManyRounds = 0;
    scanf("%d", &howManyRounds);
    if(howManyRounds > 0) {
      puts("Hello");
      break;
    } else if (enteredWithErr == 1) {
      puts("Integers only!");
      scanf("%c", &howManyPlayers); 
      enteredWithErr += 1;
    } 
  }


  //Initializing params
  initscr();
  cbreak();
  noecho();
  resizeterm(40, 80);
  start_color();
  init_pair(10, COLOR_WHITE, COLOR_BLACK);
  bkgd(COLOR_PAIR(10));
  curs_set(0);
  wborder(stdscr, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  refresh();

  int numberOfRounds = howManyRounds;

  mvprintw(1, 1, "  %d  ", numberOfRounds);
  refresh();

  

  gamearea = drawGameUI();

  playerOne = (struct playerStruct *)malloc(sizeof(struct playerStruct));
  playerTwo = (struct playerStruct *)malloc(sizeof(struct playerStruct));

  createPlayers(1, gamearea, playerNameOne, playerOne);
  createPlayers(2, gamearea, playerNameTwo, playerTwo);

  if(isRobot == true) {
    playerTwo->ai = true;
  };

  weaponsWinOne = createWeapons(playerOne, gamearea);
  weaponsWinTwo = createWeapons(playerTwo, gamearea);

  scoreCount(gamearea, playerOne);
  scoreCount(gamearea, playerTwo);

  //Initiate weapons on screen
  weaponSelector(weaponsWinOne, playerOne, playerOne->weapon, gamearea);
  weaponSelector(weaponsWinTwo, playerTwo, playerTwo->weapon, gamearea);

  gameLoop(weaponsWinOne, weaponsWinTwo, gamearea, numberOfRounds);
  

  getch();

  endwin();

  system("clear");
  printf("\033[1;33m");
  if(playerOne->points > playerTwo->points) {
    printf("The winner is: %s\n", playerOne->name);
  } else if (playerOne->points < playerTwo->points) {
    printf("The winner is: %s\n", playerTwo->name);
  } else {
    printf("It's a tie!\n");
  }

  printf("\033[0;30m");

  return 0;
}

void createPlayers(int id, WINDOW *gamearea, char *playerName, struct playerStruct *playerNow) {
  playerNow->id = id;
  playerNow->points = 0;
  playerNow->name = playerName;
  playerNow->turn = false;
  playerNow->ai = false;
  playerNow->isReady = false;
  playerNow->weapon = 0;

  int gWinX, gWinY;
  getmaxyx(gamearea, gWinY, gWinX);
  wattrset(gamearea, A_BOLD);
  start_color();

  init_pair(1, COLOR_WHITE, COLOR_MAGENTA);
  init_pair(2, COLOR_WHITE, COLOR_BLUE);

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


void weaponSelector(WINDOW *weaponsWin, struct playerStruct *playerNow, int selectedWeapon, WINDOW *gamearea) {
  int placeX, placeY, height, width, msgY, msgX, msgLen;
  placeX = 4; 
  
  wattrset(weaponsWin, A_NORMAL);
  getmaxyx(weaponsWin, height, width);
  
  for(int i = 0; i < 3; i++) {
    int len = strlen(playerNow->weapons[i]);
    if(i == selectedWeapon && playerNow->turn == true) {
      wattrset(weaponsWin, A_UNDERLINE);
      mvwprintw(weaponsWin, 3, placeX, "%s", playerNow->weapons[i]);
      wattroff(weaponsWin, A_UNDERLINE);
    } else {
      mvwprintw(weaponsWin, 3, placeX, "%s", playerNow->weapons[i]);
    }

    wrefresh(weaponsWin);
    placeX += (width / 3) - len / 2;
  }

}
void playRound(WINDOW *gamearea, node_t *AIList);

void gameLoop(WINDOW *weaponsWinOne, WINDOW *weaponsWinTwo, WINDOW *gamearea, int numberOfRounds) {
  int whoStarts, ch, msgLen;
  char turnCat[] = "'s turn";
  whoStarts = 0;

  if(playerTwo->ai == false) {
    while(whoStarts != 1 && whoStarts != 2) {
      whoStarts = rand() % 10;
      if(whoStarts == 1) {
        playerOne->turn = true;
      } else if(whoStarts == 2) {
        playerTwo->turn = true;
      }
    }
  } else {
    playerOne->turn = true;
  }
  

  weaponSelector(weaponsWinOne, playerOne, playerOne->weapon, gamearea);
  weaponSelector(weaponsWinTwo, playerTwo, playerTwo->weapon, gamearea);
  node_t *AIList = createAIList();
  
  while(1) {
    if(playerOne->turn == true && playerOne->ai == false) {
      keypad(weaponsWinOne, TRUE);
      //Upd msg playerOne
      // msgLen = strlen(playerOne->name) + (sizeof(turnCat) / sizeof(char)) + 1;
      char turnMsg[100];
      char spaces[] = "          ";
      strcpy(turnMsg, spaces);
      strcat(turnMsg, playerOne->name);
      strcat(turnMsg, turnCat);
      strcat(turnMsg, spaces);
      updateGameMsg(gamearea, turnMsg);

      ch = wgetch(weaponsWinOne);
      switch(ch) {
        case KEY_LEFT: 
          if(playerOne->weapon == 0) {
            playerOne->weapon = 2;
            weaponSelector(weaponsWinOne, playerOne, playerOne->weapon, gamearea);
          } 
          else {
            playerOne->weapon--;
            weaponSelector(weaponsWinOne, playerOne, playerOne->weapon, gamearea);
          }
          break;
        case KEY_RIGHT:
          if(playerOne->weapon == 2) {
            playerOne->weapon = 0;
            weaponSelector(weaponsWinOne, playerOne, playerOne->weapon, gamearea);
          }
          else {
            playerOne->weapon++;
            weaponSelector(weaponsWinOne, playerOne, playerOne->weapon, gamearea);
          }
          break;
        case ' ':
          keypad(weaponsWinOne, FALSE);
          playerOne->turn = false;
          playerOne->isReady = true;
          if(playerTwo->isReady == false) {
            playerTwo->turn = true;
            keypad(weaponsWinTwo, TRUE);
          } 
          weaponSelector(weaponsWinOne, playerOne, playerOne->weapon, gamearea);
          weaponSelector(weaponsWinTwo, playerTwo, playerTwo->weapon, gamearea);
          break;
      }
    } else if (playerTwo->turn == true && playerTwo->ai == false) {
      keypad(weaponsWinTwo, TRUE);
      //Upd msg playerTwo
      char turnMsg[100];
      char spaces[] = "          ";
      strcpy(turnMsg, spaces);
      strcat(turnMsg, playerTwo->name);
      strcat(turnMsg, turnCat);
      strcat(turnMsg, spaces);
      updateGameMsg(gamearea, turnMsg);

      ch = wgetch(weaponsWinTwo);
      switch(ch) {
        case KEY_LEFT: 
          if(playerTwo->weapon == 0) {
            playerTwo->weapon = 2;
            weaponSelector(weaponsWinTwo, playerTwo, playerTwo->weapon, gamearea);
          }
          else {
            playerTwo->weapon--;
            weaponSelector(weaponsWinTwo, playerTwo, playerTwo->weapon, gamearea);
          }
          break;
        case KEY_RIGHT:
          if(playerTwo->weapon == 2) {
            playerTwo->weapon = 0;
            weaponSelector(weaponsWinTwo, playerTwo, playerTwo->weapon, gamearea);
          }
          else {
            playerTwo->weapon++;
            weaponSelector(weaponsWinTwo, playerTwo, playerTwo->weapon, gamearea);
          }
          break;
        case ' ':
          keypad(weaponsWinTwo, FALSE);
          playerTwo->turn = false;
          playerTwo->isReady = true;
          if(playerOne->isReady == false) {
            playerOne->turn = true;
            keypad(weaponsWinOne, TRUE);
          }
          weaponSelector(weaponsWinTwo, playerTwo, playerTwo->weapon, gamearea);
          weaponSelector(weaponsWinOne, playerOne, playerOne->weapon, gamearea);
          break;
      }
    } else if (playerTwo->turn == true && playerTwo->ai == true) {
      char turnMsg[100];
      char spaces[] = "          ";
      strcpy(turnMsg, spaces);
      strcat(turnMsg, playerTwo->name);
      strcat(turnMsg, turnCat);
      strcat(turnMsg, spaces);
      updateGameMsg(gamearea, turnMsg);
      // playerTwo->weapon = AIplay(AIList);
      // timerFunc(1);
      // playerTwo->weapon = AIplay(AIList);
      // weaponSelector(weaponsWinTwo, playerTwo, playerTwo->weapon, gamearea);
      // timerFunc(1);
      // playerTwo->weapon = AIplay(AIList);
      // weaponSelector(weaponsWinTwo, playerTwo, playerTwo->weapon, gamearea);
      timerFunc(1);
      playerTwo->weapon = AIplay(AIList);
      weaponSelector(weaponsWinTwo, playerTwo, playerTwo->weapon, gamearea);
      timerFunc(1);

      playerTwo->turn = false;
      playerTwo->isReady = true;

      weaponSelector(weaponsWinOne, playerOne, playerOne->weapon, gamearea);
      weaponSelector(weaponsWinTwo, playerTwo, playerTwo->weapon, gamearea);

    }
    
    if(playerOne->isReady == true && playerTwo->isReady == true) {
      playRound(gamearea, AIList);
      numberOfRounds -= 1;
      mvprintw(1, 1, "  %d  ", numberOfRounds);
      refresh();
      if(numberOfRounds == 0) {
        break;
      }
      getch();
      weaponSelector(weaponsWinOne, playerOne, playerOne->weapon, gamearea);
      weaponSelector(weaponsWinTwo, playerTwo, playerTwo->weapon, gamearea);
    } 

  
  }
}

void playRound(WINDOW *gamearea, node_t *AIList) {
  int randNext;
  playerOne->isReady = false;
  playerTwo->isReady = false;

  char winMsg[100];
  char spaces[] = "          ";
  char winCat[] = " Wins";

  if((playerOne->weapon == 0 && playerTwo->weapon == 0) 
  || (playerOne->weapon == 1 && playerTwo->weapon == 1) 
  || (playerOne->weapon == 2 && playerTwo->weapon == 2)) {
    updateGameMsg(gamearea, "     It's a tie!     ");
    randNext = rand() % 2;
    if(randNext == 0) 
      playerOne->turn = true;
    else if (playerTwo->ai == false)
      playerTwo->turn = true;
    else 
      playerOne->turn = true;
    //update message win, create a timer function delay.
  } else if (playerOne->weapon == 0 && playerTwo->weapon == 1) {
    strcpy(winMsg, spaces);
    strcat(winMsg, playerTwo->name);
    strcat(winMsg, winCat);
    strcat(winMsg, spaces);
    updateGameMsg(gamearea, winMsg);
    playerTwo->points += 1;
    scoreCount(gamearea, playerTwo);
    if(playerTwo->ai == false)
      playerTwo->turn = true;
    else {
      win(AIList, playerTwo->weapon);
      playerOne->turn = true;
    }
    //update message win, create a timer function delay.
  } else if (playerOne->weapon == 0 && playerTwo->weapon == 2) {
    strcpy(winMsg, spaces);
    strcat(winMsg, playerOne->name);
    strcat(winMsg, winCat);
    strcat(winMsg, spaces);
    updateGameMsg(gamearea, winMsg);
    playerOne->points += 1;
    scoreCount(gamearea, playerOne);
    playerOne->turn = true;
    if(playerTwo->ai == true) {
      AIList = lose(&AIList, playerTwo->weapon);
    }
    //update message win, create a timer function delay.
  } else if (playerOne->weapon == 1 && playerTwo->weapon == 0) {
    strcpy(winMsg, spaces);
    strcat(winMsg, playerOne->name);
    strcat(winMsg, winCat);
    strcat(winMsg, spaces);
    updateGameMsg(gamearea, winMsg);
    playerOne->points += 1;
    scoreCount(gamearea, playerOne);
    playerOne->turn = true;
    if(playerTwo->ai == true) {
      AIList = lose(&AIList, playerTwo->weapon);
    }
  } else if (playerOne->weapon == 1 && playerTwo->weapon == 2) {
    strcpy(winMsg, spaces);
    strcat(winMsg, playerTwo->name);
    strcat(winMsg, winCat);
    strcat(winMsg, spaces);
    updateGameMsg(gamearea, winMsg);
    playerTwo->points += 1;
    scoreCount(gamearea, playerTwo);
    if(playerTwo->ai == false)
      playerTwo->turn = true;
    else {
      win(AIList, playerTwo->weapon);
      playerOne->turn = true;
    }
  } else if (playerOne->weapon == 2 && playerTwo->weapon == 0) {
    strcpy(winMsg, spaces);
    strcat(winMsg, playerTwo->name);
    strcat(winMsg, winCat);
    strcat(winMsg, spaces);
    updateGameMsg(gamearea, winMsg);
    playerTwo->points += 1;
    scoreCount(gamearea, playerTwo);
    if(playerTwo->ai == false)
      playerTwo->turn = true;
    else {
      win(AIList, playerTwo->weapon);
      playerOne->turn = true;
    }
  } else if (playerOne->weapon == 2 && playerTwo->weapon == 1) {
    strcpy(winMsg, spaces);
    strcat(winMsg, playerOne->name);
    strcat(winMsg, winCat);
    strcat(winMsg, spaces);
    updateGameMsg(gamearea, winMsg);
    playerOne->points += 1;
    scoreCount(gamearea, playerOne);
    playerOne->turn = true;
    if(playerTwo->ai == true) {
     AIList = lose(&AIList, playerTwo->weapon);
    }
  }
  

}


