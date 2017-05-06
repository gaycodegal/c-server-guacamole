#include "list.h"

/*
  Adds a node at index n of a list or at the end of the list if n is 
  too large.
  Parse the list once
*/
void add_node_at(struct s_node* node, struct s_node** head, int n){
  if(node == NULL || node->elem == NULL || head == NULL)
    return;
  if(*head == NULL || n <= 0){
    add_node(node, head);
    return;
  }
  struct s_node *last = *head;
  
  while(--n && last->next != NULL)
    last = last->next;

  node->next = last->next;
  if(last->next != NULL){
    last->next->prev = node;
  }
  node->prev = last;
  last->next = node;
}
