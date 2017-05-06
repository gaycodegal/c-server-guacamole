#include "list.h"

/* 
  Removes the given node from the list. Returns elem and frees the 
  node from memory.
  DOES NOT PARSE THE LIST!!!!!!!!!!!!!!!
*/
void* remove_node(struct s_node** node){
  if(node == NULL || *node == NULL)
    return NULL;
  struct s_node* temp = *node;
  *node = NULL;
  if(temp->next != NULL){
    temp->next->prev = temp->prev;
    *node = temp->next;
  }
  
  if(temp->prev != NULL){
    temp->prev->next = temp->next;
    *node = temp->prev;
  }
  
  void* elem = temp->elem;
  free(temp);

  return elem;
}
