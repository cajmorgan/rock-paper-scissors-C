#include <stdlib.h>
#include <stdio.h>
#include "AI.h"

int AIplay(node_t *base) {
  int playMove;
  node_t *current;
  current = base;
  shuffle(&base);
  while(current->next != NULL) {
    current = current->next;
  } 
  playMove = current->val;
  return playMove;
}

void win(node_t *base, int winMove) {
  push(base, winMove);
}

node_t *lose(node_t **base, int loseMove) {
  node_t *current;
  current = *base;

  if(loseMove == 0) {
    push(*base, 2);
  } else if (loseMove == 1) {
    push(*base, 0);
  } else if (loseMove == 2) {
    push(*base, 1);
  }

  int rocks = 0, papers = 0, scissors = 0;
  //Check how many of them left
  while(current != NULL) {
    switch(current->val) {
      case 0:
        rocks += 1;
        break;
      case 1:
        papers += 1;
        break;
      case 2:
        scissors += 1;
        break;
    }
    current = current->next;
  }
  // printf("\nRocks %d\n", rocks);
  // printf("\nPapers %d\n", papers);
  // printf("\nScissrs %d\n", scissors);

  if(loseMove == 0 && rocks > 1) {
    spliceByValue(base, loseMove);
  } else if(loseMove == 1 && papers > 1) {
    spliceByValue(base, loseMove);
  } else if(loseMove == 2 && scissors > 1) {
    spliceByValue(base, loseMove);
  }

  return *base;
}

node_t *createAIList() {
  node_t *base = NULL;
    base = (node_t *)malloc(sizeof(node_t));
    if(base == NULL)
      return NULL;

  base->val = 0;
  base->prev = NULL;
  base->next = NULL;
  push(base, 1);
  push(base, 2);

  return base;
}


void push(node_t *base, int val) {
  node_t *current = base;

  while(current->next != NULL) {
    current = current->next;
  }

  current->next = (node_t *) malloc(sizeof(node_t));
  current->next->prev = (node_t *) malloc(sizeof(node_t));
  current->next->val = val;
  current->next->next = NULL;
  current->next->prev = current;

}


void print_list(node_t *base) {
  node_t *current = base;

  while(current != NULL) {
    printf("%d\n", current->val);
    current = current->next;
  };
}


void shuffle(node_t **base) {
  int lengthIndex = 1;
  node_t *first, *last, *temp;
  first = *base;
  last = *base;
  temp = *base;

  while(first->next != NULL) {
    first = first->next;
    last = last->next;
    temp = temp->next;
    lengthIndex++;
  }


  int start = 0;
  while(lengthIndex > 1) {
    int tempVal;
    int random = (rand() % lengthIndex);

    while(random > 0) {
      first = first->prev;
      random--;
    }

    tempVal = last->val;
    last->val = first->val;
    first->val = tempVal;
    first = temp->prev;
    lengthIndex--;

  }
}

int spliceByValue(node_t **base, int searchVal) {
  int returnValue = -1;
  node_t *current, *second;
  current = *base;
  second = (*base)->next;

  if(current->val == searchVal) {
    returnValue = (*base)->val;
    *base = NULL;
    free(*base);
    *base = second;
  } else {
    while(second->val != searchVal && second->next != NULL) {
      current = current->next;
      second = second->next;
    }

    if(second->next != NULL && second->val == searchVal) {
      current->next = second->next;
      returnValue = second->val;
      second = NULL;
      free(second);
    } else if(second->next == NULL && second->val == searchVal) {
      returnValue = second->val;
      current->next = NULL;
      second = NULL;
      free(second);
    }
  }

  return returnValue;
}