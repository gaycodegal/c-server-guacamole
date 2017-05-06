#include "my.h"
#include <stdlib.h> 

/* I pledge my honor that I have 
abided by the Stevens Honor System - Steph Oro */

int my_revstr(char * str){
  int len = my_strlen(str);
  if(len <= 0) return len;
  char * last = str + (len - 1), temp;
  while(str < last){
    temp = *str;
    *(str++) = *last;
    *(last--) = temp;
  }
  return len;
}
