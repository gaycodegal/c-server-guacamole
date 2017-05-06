#include "my.h"
#include <stdlib.h> 

/* I pledge my honor that I have abided
   by the Stevens Honor System - Steph Oro */
/*
  Prints a number using the length of the
  string as the base and the contents as
  the alphabet. For example, if you called 
  my_num_base(9, "RTFM"), then R = 0, T = 1,
  F = 2, M = 3. 9 in base 4 is 21 and so the
  result is printed out as "FT".
  BE PARANOID.
  If char* is NULL or empty, print an error 
  message and return.
  If given unary, repeat alphabet letter the
  specified number of times
  For negatives print a '-' and then the letters.
*/
void my_num_base(int x, const char* base){
  int od = 1, ev = 1, m = 1, len, i, temp;
  if(base == NULL){
    my_str("ERROR: NULL STRING\n");
    return;
  }
  len = my_strlen(base);
  if(len == 0){
    my_str("ERROR: BASE 0\n");
    return;
  }else if(len == 1){
    if(x < 0){
      my_char('-');
      for(i = 0; x < i; ++x){
	my_char(base[0]);
      }
    }
    for(i = 0; i < x; ++i){
      my_char(base[0]);
    }
    return;
  }else if(x == 0){
    my_char(base[0]);
    return;
  }
	
  if(x < 0){
    m = -1;
    my_char('-');
  }
  temp = len - 1;
  while(x > temp || x < -temp){
    od = od*len + (x%len)*m;
    x = x/len;
    ev = ev*len + (x%len)*m;
    x = x/len;
  }
  if(x)
    my_char(base[x*m]);
  while(od > 1 || ev > 1){
    my_char(base[ev%len]);
    ev = ev / len;
    my_char(base[od%len]);
    od = od / len;
  }
}
