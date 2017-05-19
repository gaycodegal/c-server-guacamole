#include "list.h"

/*
  allocates a pointer to an empty list
 */
struct s_node ** make_empty_list(){
  struct s_node ** queue = (struct s_node **)malloc(sizeof(struct s_node *));
  *queue = NULL;
  return queue;
}
