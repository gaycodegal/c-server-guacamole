#include "my.h"
#include <stdlib.h>



/* I pledge my honor that I have abided 
by the Stevens Honor System - Steph Oro */
/*
**Allocates new memory.**
Copies concatenated strings in that new memory
Returns pointer to it
If both NULL return NULL
Returns new memory, with contents of a
followed by contents of b
* Always copies a '\0'
*/
char * my_strconcat(const char * str1, const char * str2){
  char * p, c;
  if(str2 == NULL && str1 == NULL)
    return NULL;
  int len1 = my_strlen(str1), len2 = my_strlen(str2);
  int len = 1;
  if(len1 > 0)
    len += len1;
  if(len2 > 0)
    len += len2;
  char * newstr = (char *)malloc(sizeof(char) * len);
  p = newstr;
  if(str1 != NULL){
    while((c = *str1)){
      *(p++) = c;
      ++str1;
    }
  }
  if(str2 != NULL){
    while((c = *str2)){
      *(p++) = c;
      ++str2;
    }
  }
  *p = 0;
  return newstr;
}
