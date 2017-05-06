#include "my.h"
#include <stdlib.h> 

/* I pledge my honor that I have abided
by the Stevens Honor System - Steph Oro */

void my_str(const char * str){
  if(str == NULL) return;
  char c;
  while((c = *(str++)))
      my_char(c);
}
