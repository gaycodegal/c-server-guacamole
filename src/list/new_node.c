#include "list.h"

/*
  Allocates and returns a node with the given element. You may
  create nodes with NULL element.
*/
struct s_node* new_node(void* elem){
  struct s_node* node = (struct s_node*)malloc(sizeof(struct s_node));
  node->next = NULL;
  node->prev = NULL;
  node->elem = elem;
  return node;
}
