#include "my.h"
#include "list.h"
#include <stdlib.h>

void * pointershift(void *p, int size, int places){
  p = (void *)(((char *)p) + size*places);
  return p;
}

void print_as(void * t, int kind){
  switch (kind){
  case 1:
    my_str((char *)t);
    break;
  case 2:
    my_int(*(int *)t);
    break;
  default:
    my_char(*(char *)t);
    break;
  };
}

void test(void * symbols, int size, int kind, void (* printer)(struct s_node * arg), void (* debugger)(struct s_node * arg)){
  struct s_node * origin = new_node(symbols);
  struct s_node ** list = &origin;
  struct s_node * NULL_LIST[1];
  struct s_node * NULL_HOLDER[1];
  my_str("malloc[");
  struct s_node * a = new_node(pointershift(symbols,size,3));
  my_char('.');
  struct s_node * b = new_node(pointershift(symbols,size,4));
  my_char('.');
  struct s_node * c = new_node(pointershift(symbols,size,3));
  my_char('.');
  struct s_node * d = new_node(pointershift(symbols,size,4));
  my_char('.');
  struct s_node * e = new_node(pointershift(symbols,size,5));
  my_char('.');
  struct s_node * f = new_node(pointershift(symbols,size,6));
  my_char('.');
  struct s_node * g = new_node(pointershift(symbols,size,7));
  my_char('.');
  struct s_node * h = new_node(pointershift(symbols,size,8));
  my_char('.');
  struct s_node * k = new_node(pointershift(symbols,size,9));
  my_char('.');
  my_str("]\n");
  struct s_node * NULL_NODE = new_node(NULL);
  *NULL_LIST = NULL;
  int t1;//, t2, t3;

  *NULL_HOLDER = NULL_NODE;
  
  //test creating a list works
  my_str("test new_node:\n");
  debugger(*list);
  my_str("\n");

  //test add_elem
  my_str("test add_elem:\n");
  add_elem(pointershift(symbols,size,1), list);
  debugger(*list);
  my_str("\n");
  add_elem(pointershift(symbols,size,2), list);
  debugger(*list);
  my_str("\n");
  add_elem(NULL, list);
  debugger(*list);
  my_str("\n");
  add_elem(symbols, NULL);
  debugger(*list);
  my_str("\n");
  add_elem(symbols, NULL_LIST);
  debugger(*NULL_LIST);
  my_str("\n");
  empty_list(NULL_LIST);
  add_elem(NULL, NULL);
  debugger(*list);
  my_str("\n");
  add_elem(NULL, NULL_LIST);
  debugger(*list);
  my_str("\n");
  
  //test add_node
  my_str("test add_node:\n");
  add_node(b, list);
  add_node(a, list);
  debugger(*list);
  my_str("\n");
  add_node(NULL, list);
  debugger(*list);
  my_str("\n");
  add_node(a, NULL);
  debugger(*list);
  my_str("\n");
  add_node(NULL, NULL);
  debugger(*list);
  my_str("\n");
  add_node(NULL_NODE, list);
  debugger(*list);
  my_str("\n");
  add_node(NULL_NODE, NULL);
  debugger(*list);
  my_str("\n");
  add_node(NULL, NULL_LIST);
  debugger(*NULL_LIST);
  my_str("\n");
  add_node(NULL_NODE, NULL_LIST);
  debugger(*NULL_LIST);
  my_str("\n");
  add_node(new_node(symbols), NULL_LIST);
  debugger(*NULL_LIST);
  my_str("\n");
  empty_list(NULL_LIST);
 
  //test append
  my_str("test append:\n");
  append(c, list);
  append(d, list);
  debugger(*list);
  my_str("\n");
  append(NULL, list);
  debugger(*list);
  my_str("\n");
  append(a, NULL);
  debugger(*list);
  my_str("\n");
  append(NULL, NULL);
  debugger(*list);
  my_str("\n");
  append(NULL_NODE, list);
  debugger(*list);
  my_str("\n");
  append(NULL_NODE, NULL);
  debugger(*list);
  my_str("\n");
  append(NULL, NULL_LIST);
  debugger(*NULL_LIST);
  my_str("\n");
  append(NULL_NODE, NULL_LIST);
  debugger(*NULL_LIST);
  my_str("\n");
  append(new_node(symbols), NULL_LIST);
  debugger(*NULL_LIST);
  my_str("\n");
  empty_list(NULL_LIST);
  
  //test add_node_at
  my_str("test add_node_at:\n");
  add_node_at(e, list,-1);
  debugger(*list);
  my_str("\n");
  add_node_at(k, list,1);
  debugger(*list);
  my_str("\n");
  add_node_at(f, list,8);
  debugger(*list);
  my_str("\n");
  add_node_at(g, list,10);
  debugger(*list);
  my_str("\n");
  add_node_at(h, list,0);
  debugger(*list);
  my_str("\n");
  add_node_at(NULL, list, 3);
  debugger(*list);
  my_str("\n");
  add_node_at(a, NULL, 3);
  debugger(*list);
  my_str("\n");
  add_node_at(NULL, NULL, 3);
  debugger(*list);
  my_str("\n");
  add_node_at(NULL_NODE, list, 3);
  debugger(*list);
  my_str("\n");
  add_node_at(NULL_NODE, NULL, 3);
  debugger(*list);
  my_str("\n");
  add_node_at(NULL, NULL_LIST, 3);
  debugger(*NULL_LIST);
  my_str("\n");
  add_node_at(NULL_NODE, NULL_LIST, 3);
  debugger(*NULL_LIST);
  my_str("\n");
  add_node_at(new_node(symbols), NULL_LIST, 3);
  debugger(*NULL_LIST);
  my_str("\n");
  empty_list(NULL_LIST);

  //test count_s_nodes
  my_str("test count_s_nodes:\n");  
  my_int((t1 = count_s_nodes(*list)));
  my_str("\n");
  my_int(count_s_nodes(NULL));
  my_str("\n");
  my_int(count_s_nodes(NULL_NODE));
  my_str("\n");

  //test elem_at
  my_str("test elem_at:\n");  
  for(int i = -1; i < t1 + 1; ++i){
    my_int(i);
    my_str(": ");
    print_as(elem_at(*list, i), kind);
    my_str("\n");
  }
  if(elem_at(NULL_NODE, 0) == NULL)
    my_str("null retrieved 1\n");
  if(elem_at(NULL, 0) == NULL)
    my_str("null retrieved 2\n");
  
  //test node_at
  my_str("test node_at:\n");  
  for(int i = -1; i < t1 + 1; ++i){
    my_int(i);
    my_str(": ");
    printer(node_at(*list, i));
    my_str("\n");
  }
  if(node_at(NULL_NODE, 0) == NULL_NODE)
    my_str("null retrieved 1\n");
  if(node_at(NULL, 0) == NULL)
    my_str("null retrieved 2\n");  

  //test remove_node_at
  my_str("test remove_node_at:\n");  
  if(remove_node_at(NULL_LIST, 3) == NULL)
    my_str("rem null good\n");
  if(remove_node_at(NULL, 3) == NULL)
    my_str("rem null good\n");
  print_as((remove_node_at(list, 0)), kind);
  my_str("\n");
  debugger(*list);
  my_str("\n");
  print_as((remove_node_at(list, -1)), kind);
  my_str("\n");
  debugger(*list);
  my_str("\n");
  print_as((remove_node_at(list, 9)), kind);
  my_str("\n");
  debugger(*list);
  my_str("\n");
  print_as((remove_node_at(list, 9)), kind);
  my_str("\n");
  debugger(*list);
  my_str("\n");
  print_as((remove_node_at(list, 4)), kind);
  my_str("\n");
  debugger(*list);
  my_str("\n");

  //test remove_last
  my_str("test remove_last:\n");  
  if(remove_last(NULL_LIST) == NULL)
    my_str("rem null good\n");
  if(remove_last(NULL) == NULL)
    my_str("rem null good\n");
  print_as((remove_last(list)), kind);
  my_str("\n");  
  debugger(*list);
  my_str("\n");
  print_as((remove_last(list)), kind);
  my_str("\n");  
  debugger(*list);
  my_str("\n");
  
  //test remove
  my_str("test remove:\n");  
  if(remove_node(NULL_HOLDER) == NULL)
    if(*NULL_HOLDER == NULL)
      my_str("remove ok\n");
  *NULL_HOLDER = node_at(*list, 1);
  print_as((remove_node(NULL_HOLDER)), kind);
  my_str("\n");  
  debugger(*list);
  my_str("\n");

  empty_list(list);
  
  //free(origin);

}

int main(){
  int numbers[] = {1,2,3,4,5,0,-1,-2,-3, 0x7FFFFFFF};
  char chars[] = {'a','b','c','d','e','f','g','h','i', 'j'};
  const char * strs = "aba\0bcb\0cdc\0ded\0efe\0fgf\0ghg\0gig\0iji\0jkj\0";
  test(numbers, sizeof(int), 2, print_int, debug_int);
  test(numbers, sizeof(int), 2, print_int, debug_int);
  test(chars, sizeof(char), 3, print_char, debug_char);
  test(chars, sizeof(char), 3, print_char, debug_char);
  test((char *)strs, sizeof(char) * 4, 1, print_string, debug_string);
  test((char *)strs, sizeof(char) * 4, 1, print_string, debug_string);
  
  return 0;
}
