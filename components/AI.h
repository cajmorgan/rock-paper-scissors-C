 typedef struct node {
    int val;
    struct node *next;
    struct node *prev;
  } node_t;

  node_t *createAIList();
  void push(node_t *base, int val);
  void print_list(node_t *base);
  void shuffle(node_t **base);
  void win(node_t *base, int winMove);
  void lose(node_t *base, int loseMove);
  int spliceByValue(node_t **base, int searchVal); 