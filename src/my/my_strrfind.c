#include "my.h"
#include <stdlib.h> 

/* I pledge my honor that I have abided by
the Stevens Honor System - Steph Oro */
/*
Returns a pointer to the last char in the
string which matches. Return NULL if the 
letter is not found/input string is null.
*/
char * my_strrfind(char * str, char c){
  char tmp = 0;
  char * ret = NULL;
  if(str == NULL) 
    return ret;
  int len = 0;
  while((tmp = *str)){
    if(tmp == c)
      ret = str;
    ++len;
    ++str;
  }
  
  if(c == '\0')
    ret = str;
        
  return ret;
}
