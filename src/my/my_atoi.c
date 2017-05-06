#include "my.h"
#include <stdlib.h>


/* I pledge my honor that I have abided by
the Stevens Honor System - Steph Oro */
/*
Returns the int represented by the ascii
string
Handles negatives.
Ignores preceding characters and trailing
numbers and chars
Does not allocate memory 

Examples:
 "5" => 5
 "-5" => -5
 "--5" => 5
 "a-b54sc7-d" => -54
 "abcd" => 0
 “a-b-54sc85-ed” =>54
 NULL=>0
 “”=>0
 “0000” =>0
*/
int my_atoi(const char * str){
  if(str == NULL) return 0;
  char c;
  int v = 0;
  while((c=*str) && c != '-' && (c < '0' || c > '9')){
    ++str;
  }
  int m = 1;
  if(c == '-')
    m = -1;
  while((c=*str) && (c < '0' || c > '9')){
    ++str;
  }
  
  while((c=*str) && (c >= '0' && c <= '9')){
    v = v*10 + m*(c-'0');
    ++str;
  }
  
  return v;
}
