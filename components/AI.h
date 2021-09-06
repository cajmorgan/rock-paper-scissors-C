 typedef struct node {
    int val;
    struct node *next;
  } node_t;

  node_t *createAIList();
  void shuffleList(node_t *AIList);