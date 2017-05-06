#include "my.h"
#include <stdlib.h> 

/* I pledge my honor that I have abided
by the Stevens Honor System - Steph Oro */
/*
Returns a pointer to the first char in 
the string which matches. Return NULL 
if the letter is not found/input string is null.
*/
char * my_strfind(char * str, char c){
  char tmp = 0;
  if(str == NULL) 
    return NULL;
  int len = 0;
  while((tmp = *str)){
    if(tmp == c)
      return str;
    ++len;
    ++str;
  }
  
  if(c == '\0')
    return str;
        
  return NULL;
}
