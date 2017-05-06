#include <stdlib.h>
#include "my.h"
#include "list.h"

/*
  Prints the elem of node as an int
*/
void print_int(struct s_node* node){
  if(node == NULL || node->elem == NULL){
    my_str("NULL");
    return;
  }

  my_int(*((int *)node->elem));
}
