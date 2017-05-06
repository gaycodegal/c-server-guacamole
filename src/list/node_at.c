#include "list.h"

/*
  Returns a pointer to the node at index n or the last node.
  Parse once
 */
struct s_node* node_at(struct s_node* head, int n){
  if(head == NULL)
    return NULL;

  if(n <= 0)
    return head;
  
  while(n-- && head->next != NULL){
    head = head->next;
  }
  
  return head;
}
