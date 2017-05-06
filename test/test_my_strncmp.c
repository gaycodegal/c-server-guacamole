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

int main(){
  printf("TESTING my_strncmp\n");
  int maxstrlen = 20;
  /*int testc[] = {-1,1,1,-1,
              0,0,1,-1,
                 -1,1,1,-1,
              0,0,-1,1,
                 0,-1,1,0,
              0,0,1,-1,
                 0,0,0,-1,
              1,-1,1,1,
                 -1,1,-1,0,
              0,0,0,0};*/
  int testL[] = {0,0,2,2,
              7,7,7,7,
                 14,14,14,14,
              -1,-1,-1,-1,
                 10,10,10,14,
              0,1,1,1,
                 0,0,0,3,
              3,2,2,0,
                 0,1,1,3,
              3,13,15,20
                };
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
  strcpy(tests[ti++], "ny dog is fine");//7
    
  strcpy(tests[ti++], "my dog is find");
  strcpy(tests[ti++], "my dog is fine");
    
  strcpy(tests[ti++], "my dog is fine");
  strcpy(tests[ti++], "my dog is find");
    
  strcpy(tests[ti++], "ny dog is fine");
  strcpy(tests[ti++], "my dog is fine");
    
  strcpy(tests[ti++], "my dog is fine");
  strcpy(tests[ti++], "ny dog is fine");//14
    
  strcpy(tests[ti++], "my dog is find");
  strcpy(tests[ti++], "my dog is fine");
    
  strcpy(tests[ti++], "my dog is fine");
  strcpy(tests[ti++], "my dog is find");
    
  strcpy(tests[ti++], "ny dog is ");
  strcpy(tests[ti++], "my dog is fine");
    
  strcpy(tests[ti++], "my dog is fine");
  strcpy(tests[ti++], "ny dog is ");// -1
    
  tests[ti++] = NULL;
  tests[ti++] = NULL;

  tests[ti++] = NULL;
  strcpy(tests[ti++], "CAsdfsAFDasdf");

  strcpy(tests[ti++], "CAsdfsAFDasdf");
  tests[ti++] = NULL;
  
  strcpy(tests[ti++], "my dog is fine");
  strcpy(tests[ti++], "my dog is fine"); //14
  
  strcpy(tests[ti++], "");
  strcpy(tests[ti++], "");

  strcpy(tests[ti++], "a");
  strcpy(tests[ti++], "a");
  
  strcpy(tests[ti++], "b");
  strcpy(tests[ti++], "a");

  strcpy(tests[ti++], "a");
  strcpy(tests[ti++], "b");//abc < abcdefg

  strcpy(tests[ti++], "a");
  strcpy(tests[ti++], "a");
  
  strcpy(tests[ti++], "b");
  strcpy(tests[ti++], "a");

  strcpy(tests[ti++], "a");
  strcpy(tests[ti++], "b");//abc < abcdefg

  strcpy(tests[ti++], "abc");
  strcpy(tests[ti++], "abcdefg");
  
  strcpy(tests[ti++], "abcdefg");
  strcpy(tests[ti++], "abc");

  strcpy(tests[ti++], "abc");
  strcpy(tests[ti++], "abcc");
  
  strcpy(tests[ti++], "abcc");
  strcpy(tests[ti++], "abc");

  strcpy(tests[ti++], "abc");
  strcpy(tests[ti++], "");
  
  strcpy(tests[ti++], "");
  strcpy(tests[ti++], "abc");

  strcpy(tests[ti++], "a");
  strcpy(tests[ti++], "");
  
  strcpy(tests[ti++], "");
  strcpy(tests[ti++], "a");
  
  strcpy(tests[ti++], "abccdefe");
  strcpy(tests[ti++], "abcfffff");
  
  strcpy(tests[ti++], "abcfffff");
  strcpy(tests[ti++], "abccdefe");
  
  strcpy(tests[ti++], "my dog is fine");
  strcpy(tests[ti++], "my dog is fine");
  
  strcpy(tests[ti++], "my dog is fine");
  strcpy(tests[ti++], "my dog is fine");
  
  strcpy(tests[ti++], "my dog is fine");
  strcpy(tests[ti++], "my dog is fine");

  for(int i = 0; i < TESTLEN; ++i){
    str_prnt(tests[i*2]);
    printf(" ?= (%i) ", testL[i]);
    str_prnt(tests[i*2 + 1]);
    printf(" -> %i\n", my_strncmp(tests[i*2],tests[i*2 + 1], testL[i]));
    //printf(" '%c' -> ", p);
    //str_prnt(my_strrfind(tests[i], testc[i]));
    //printf("\n");
  }
  
  for(int i = 0; i < TESTLEN * 2; ++i){
    free(tests[i]);
  }
}
