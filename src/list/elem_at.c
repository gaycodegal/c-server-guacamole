#include "list.h"

/*
  Returns a pointer to the elem at index n or the last node.
  Parse once.
*/
void* elem_at(struct s_node* head, int n){
  if(head == NULL)
    return NULL;
  struct s_node* temp = node_at(head, n);
  if(temp == NULL)
    return NULL;
  return temp->elem;
}
