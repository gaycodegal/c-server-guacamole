#include "my.h"
#include <stdlib.h> 

/* I pledge my honor that I have abided
by the Stevens Honor System - Steph Oro */

int my_strlen(const char * str){
  if(str == NULL) return -1;
  int len = 0;
  while(*(str++))
      len++;
  return len;
}
