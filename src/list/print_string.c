#include "list.h"

/*
  Prints the elem of node as a string
*/
void print_string(struct s_node* node){
  if(node == NULL || node->elem == NULL){
    my_str("NULL");
    return;
  }
  
  my_str((char *)node->elem);
}
