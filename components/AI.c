#include <stdlib.h>
#include "AI.h"

int AIplay() {


  int random;

  random = rand() % 3;

  return random;
}

node_t *createAIList() {
  node_t *base = NULL;
    base = (node_t *)malloc(sizeof(node_t));
    if(base == NULL)
      return NULL;
    
  base->val = 0;
  base->next = (node_t *)malloc(sizeof(node_t));
  base->next->val = 1;
  base->next->next = (node_t *)malloc(sizeof(node_t));
  base->next->next->val = 2;
  base->next->next->next = NULL;
  
  return base;
}

void shuffleList(node_t *AIList) {
  
}
