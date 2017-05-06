#include "list.h"

/*
  Removes the last node from the list.
  Parse Once.
*/
void* remove_last(struct s_node** head){
  if(head == NULL || *head == NULL)
    return NULL;

  struct s_node *last = *head;

  if((*head)->next == NULL){
    return remove_node(head);
  }
    
  while(last->next != NULL)
    last = last->next;

  return remove_node(&last);
}
