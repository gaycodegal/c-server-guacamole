#include "my.h"
#include <string.h>
#include <stdio.h>
#define TESTLEN 40

void str_prnt(char * str){
  if(str == NULL)
    printf("(null)");
  else
    printf("\"%s\"",str);
}

void test(char * str, int size, char * initial_value, int donull){
  char * temp = (char *)malloc(sizeof(char) * (size+1));
  if(!donull)
    strcpy(temp,initial_value);
  temp[size] = 0;
  str_prnt(str);
  printf(" over ");
  if(donull){
    free(temp);
    temp = NULL;
  }
  str_prnt(temp);
  
  my_strcpy(temp, str);
  printf(" is ");
  str_prnt(temp);
  printf("\n");
  if(!donull)
    free(temp);
}

int main(){
  printf("TESTING my_strcpy\n");
  
  test("",3,"NON",0);
  test("a",3,"NON",0);
  test("ab",3,"NON",0);
  test("abc",3,"NON",0);
  test(NULL,12,"not replaced",0);
  test("abc",12,"not replaced",1);
  test(NULL,12,"not replaced",1);
  
}
