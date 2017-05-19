#include "list.h"

/*
  Empties the list and then frees the list pointer
 */
void free_list(struct s_node ** head){
  if(head == NULL) return;
  empty_list(head);
  free(head);
}
