#include "list.h"

/*
Creates a new node with elem and adds it to head (again, assume head is actually the head). DO NOT add a NULL elem to the list
  DOES NOT PARSE THE LIST
 */
void add_elem(void* elem, struct s_node** head){
  if(elem == NULL || head == NULL)
    return;
  add_node(new_node(elem), head);
}
