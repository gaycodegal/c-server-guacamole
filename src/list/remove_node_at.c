#include "list.h"

/*
  Removes the node at index n or the last node.
  Parse once
*/
void* remove_node_at(struct s_node** head, int n){
  if(head == NULL || *head == NULL)
    return NULL;
  
  struct s_node *last = *head;

  if(n <= 0 || last->next == NULL){
    return remove_node(head);    
  }
  
  while(n-- && last->next != NULL)
    last = last->next;

  return remove_node(&last);
}
