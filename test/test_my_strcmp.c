#include "my.h"
#include <string.h>
#include <stdio.h>
#define TESTLEN 16

void str_prnt(char * str){
  if(str == NULL)
    printf("(null)");
  else
    printf("\"%s\"",str);
}

int main(){
  printf("TESTING my_strcmp\n");
  int maxstrlen = 20;
  /*int testc[] = {-1, 1, 1, -1,
                -1,1,1,-1,
                0,-1,1,0,
                 0,0,1,-1};*/
  char * tests[TESTLEN * 2];
  for(int i = 0; i < TESTLEN * 2; ++i){
    tests[i] = (char *)malloc(sizeof(char) * maxstrlen);
  }
  int ti = 0;
  strcpy(tests[ti++], "");
  strcpy(tests[ti++], "my dog is fine");
  
  strcpy(tests[ti++], "my dog is fine");
  strcpy(tests[ti++], "");
  
  strcpy(tests[ti++], "my do");
  strcpy(tests[ti++], "aasdf");
  
  strcpy(tests[ti++], "aasdf");
  strcpy(tests[ti++], "my do");//
    
  strcpy(tests[ti++], "my dog is find");
  strcpy(tests[ti++], "my dog is fine");
    
  strcpy(tests[ti++], "my dog is fine");
  strcpy(tests[ti++], "my dog is find");
    
  strcpy(tests[ti++], "ny dog is fine");
  strcpy(tests[ti++], "my dog is fine");
    
  strcpy(tests[ti++], "my dog is fine");
  strcpy(tests[ti++], "ny dog is fine");
    
  tests[ti++] = NULL;
  tests[ti++] = NULL;

  tests[ti++] = NULL;
  strcpy(tests[ti++], "CAsdfsAFDasdf");

  strcpy(tests[ti++], "CAsdfsAFDasdf");
  tests[ti++] = NULL;
  
  strcpy(tests[ti++], "my dog is fine");
  strcpy(tests[ti++], "my dog is fine");
  
  strcpy(tests[ti++], "");
  strcpy(tests[ti++], "");

  strcpy(tests[ti++], "a");
  strcpy(tests[ti++], "a");
  
  strcpy(tests[ti++], "b");
  strcpy(tests[ti++], "a");

  strcpy(tests[ti++], "a");
  strcpy(tests[ti++], "b");


  for(int i = 0; i < TESTLEN; ++i){
    str_prnt(tests[i*2]);
    printf(" ?= ");
    str_prnt(tests[i*2 + 1]);
    printf(" -> %i\n",my_strcmp(tests[i*2],tests[i*2 + 1]));
    //printf(" '%c' -> ", p);
    //str_prnt(my_strrfind(tests[i], testc[i]));
    //printf("\n");
  }
  
  for(int i = 0; i < TESTLEN * 2; ++i){
    free(tests[i]);
  }
}
