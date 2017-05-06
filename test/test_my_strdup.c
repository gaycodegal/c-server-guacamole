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

void test(char * str){
  char * dup = my_strdup(str);
  str_prnt(str);
  printf(" -> ");
  str_prnt(dup);
  free(dup);
  printf("\n");
}

int main(){
  printf("TESTING my_strdup\n");
  
  test("asdfn");
  test("pasinf");
  test("paisdnf");
  test("pasindf");
  test("pa");
  test("spdfpsainf");
  test("pidnf");
  test("paindf");
  test("pasindf");
  test("piansdfpi1n43p");
  test("i2n3t12p3tn12p");
  test("3itn12");
  test("p3t12");
  test("3t12n3tp");
  test("12in3t");
  test("12t21n");
  test(NULL);
  test("");
  
}
