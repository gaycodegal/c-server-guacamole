#include "list.h"

/*
  Adds a node to the end of a list. DO NOT add a NULL node pointer or
  a node with a NULL elem.
  Parse the list once
*/
void append(struct s_node* node, struct s_node** head){
  if(node == NULL || node->elem == NULL || head == NULL)
    return;
  struct s_node *last = *head;

  if(*head == NULL){
    *head = node;
    return;
  }
  
  while(last->next != NULL)
    last = last->next;

  node->next = last->next;
  if(last->next != NULL){
    last->next->prev = node;
  }
  node->prev = last;
  last->next = node;
}
