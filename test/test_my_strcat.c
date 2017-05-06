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

void test(char * str1, char * str2, char * str3){
  int len = 1, len1 = 0;
  if(str1 != NULL){
    len += (len1 = strlen(str1));
  }
  if(str2 != NULL){
    len += strlen(str2);
  }
  char * test = (char *)malloc(sizeof(char)*(len));
  if(str1 != NULL)
    strcpy(test, str1);
  test[len1] = 0;
  if(str1 == NULL){
    free(test);
    test = NULL;
  }
  str_prnt(str1);
  printf(" + ");
  str_prnt(str2);
  printf(" = ");
  str_prnt(my_strcat(test,str2));
  printf("\n");
  if(str1 != NULL)
    free(test);
}

int main(){
  printf("TESTING my_strcat\n");
  
  test("","","");
  test("a","","a");
  test("","a","a");
  test("a","b","ab");
  test("b","a","ba");
  test("ac","b","acb");
  test("b","ac","bac");
  test("ac","bc","acbc");
  test("bc","ac","bcac");
  test(NULL,"a",NULL);
  test("b",NULL,"b");
  test(NULL,NULL,NULL);
  
}
