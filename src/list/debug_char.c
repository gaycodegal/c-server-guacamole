#include "list.h"

/*
  Prints all the elems as chars separated by a space in the format
  (NULL <- Elem -> Next elem), ..., (Previous elem <- Elem -> NULL) 
*/
void debug_char(struct s_node* node){
  if(node == NULL)
    return;
  struct s_node *prev = node->prev, *next = node->next;
  while(node != NULL){
    next = node->next;
    my_str("(");
    print_char(prev);
    my_str(" <- ");
    print_char(node);
    my_str(" -> ");
    print_char(next);
    my_str(")");
    if(next != NULL)
      my_str(", ");
    prev = node;
    node = next;
  }
}
