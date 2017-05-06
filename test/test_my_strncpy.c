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

void test(char * str, int size, char * initial_value, int donull, int n){
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
  
  my_strncpy(temp, str, n);
  printf(" is ");
  str_prnt(temp);
  printf("\n");
  if(!donull)
    free(temp);
}

int main(){
  printf("TESTING my_strncpy\n");
  
  test("",3,"NON",0, 2);
  test("a",3,"NON",0, 2);
  test("ab",3,"NON",0, 2);
  test("abc",3,"NON",0, 2);
  test(NULL,12,"not replaced",0, 2);
  test("abc",12,"not replaced",1, 2);
  test(NULL,12,"not replaced",1, 2);
  
  test("",3,"NON",0, 3);
  test("a",3,"NON",0, 3);
  test("ab",3,"NON",0, 3);
  test("abc",3,"NON",0, 3);
  test(NULL,12,"not replaced",0, 3);
  test("abc",12,"not replaced",1, 3);
  test(NULL,12,"not replaced",1, 3);
  
  test("",3,"NON",0, 4);
  test("a",3,"NON",0, 4);
  test("ab",3,"NON",0, 4);
  test("abc",3,"NON",0, 4);
  test(NULL,12,"not replaced",0, 4);
  test("abc",12,"not replaced",1, 4);
  test(NULL,12,"not replaced",1, 4);
  
}


