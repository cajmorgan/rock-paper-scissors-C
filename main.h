#include <curses.h>

struct playerStruct {
    int id;
    int points;
    char *name;
    bool turn;
    bool ai;
    int weapon;
    char **weapons;
  } *playerOne, *playerTwo;



void createPlayers(int id, WINDOW *gamearea, char *playerName, struct playerStruct *playerNow);
WINDOW *createWeapons(struct playerStruct *playerNow, WINDOW *gamearea);
void weaponSelector(WINDOW *weaponsWin, struct playerStruct *playerNow, int selectedWeapon);
void gameLoop(WINDOW *weaponsWinOne, WINDOW *weaponsWinTwo);