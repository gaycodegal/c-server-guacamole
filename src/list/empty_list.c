#include "list.h"
/*
  Frees all the nodes in the list.
  CHALLENGE: Write in two lines (allowed and encouraged to use other methods in 
  this assignment)
*/
void empty_list(struct s_node** head){
  if(head == NULL || *head == NULL) return;
  do{remove_node(head);}while(*head != NULL);
}
