#include "list.h"
/*
  maps in place over the list
  starts at head and goes to end.
  replaces the elems in this list with the returned value;
*/
void static_map_list(struct s_node* head, void* (* map_fn)(void * elem)){
  if(head == NULL) return;
  do{
    head->elem = map_fn(head->elem);
    head = head->next;
  }while(head != NULL);
}
