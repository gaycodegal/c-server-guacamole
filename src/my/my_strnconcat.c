#include "my.h"
#include <stdlib.h>

/* I pledge my honor that I have abided by
the Stevens Honor System - Steph Oro */
/*
Same as my_strconcat except:
Copies all of a and then n chars or length of b
* Always copies a '\0' at end of concatted string
Treat n<0 as 0, and n>|b| as |b|
*/
char * my_strnconcat(const char * str1, const char * str2, int n){
  char * p, c;
  if(str2 == NULL && str1 == NULL)
    return NULL;
  int len1 = my_strlen(str1), len2 = my_strlen(str2);
  if(n < 0)
    n = 0;
  if(len2 < n)
    len2 = n;
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
    while((n > 0) && (c = *str2)){
      *(p++) = c;
      ++str2;
      --n;
    }
  }
  *p = 0;
  return newstr;
}
