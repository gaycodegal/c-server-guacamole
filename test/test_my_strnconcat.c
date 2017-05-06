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

void test(char * str1, char * str2, char * str3, int n){
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
  printf(" %i = ",n);
  char * t = my_strnconcat(test, str2, n);
  str_prnt(t);
  free(t);
  printf("\n");
  if(str1 != NULL)
    free(test);
}

int main(){
  printf("TESTING my_strnconcat\n");
  
  test("","","", 2);
  test("a","","a", 2);
  test("","a","a", 3);
  test("a","b","ab", 3);
  test("b","a","ba", 2);
  test("ac","b","acb", 1);
  test("b","ac","bac", 3);
  test("ac","bc","acbc", 2);
  test("bc","ac","bcac", 2);
  test(NULL,"a",NULL, 1);
  test("b",NULL,"b", 1);
  test(NULL,NULL,NULL, 1);
  test("","aaa","a", 1);
  test("","aaa","aa", 2);
  test("","aaa","", 0);
  test("","aaa","", -1);
}
