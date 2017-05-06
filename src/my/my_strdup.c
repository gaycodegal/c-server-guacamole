#include "my.h"
#include <stdlib.h>

/* I pledge my honor that I have abided
by the Stevens Honor System - Steph Oro */
/*
**Allocates new memory**
 Copies str into that new memory 
 returns pointer to new string
*/
char * my_strdup(const char * str1){
  char * p, c;
  if(str1 == NULL)
    return NULL;
  int len = my_strlen(str1) + 1;
  char * newstr = (char *)malloc(sizeof(char) * len);
  p = newstr;
  
  while((c = *str1)){
    *(p++) = c;
    ++str1;
  }
  
  *p = 0;
  return newstr;
}
