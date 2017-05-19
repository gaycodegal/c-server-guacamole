#include "list.h"

/*
  Returns a pointer to the last node.
 */
struct s_node* last_node(struct s_node* head){
  if(head == NULL)
    return NULL;

  while(head->next != NULL){
    head = head->next;
  }
  
  return head;
}
