#include "my.h"
#include <string.h>
#include <stdio.h>
#define TESTLEN 13

int main(){
    printf("TESTING my_strrindex\n");
    char p;
    int maxstrlen = 20;
    char testc[] = "Oo2nemmmmACa\0";
    char * tests[TESTLEN];
    for(int i = 0; i < TESTLEN; ++i){
        tests[i] = (char *)malloc(sizeof(char) * maxstrlen);
    }
    int ti = 0;
    strcpy(tests[ti++], "my dog is fine");
    strcpy(tests[ti++], "my dog is fine");
    strcpy(tests[ti++], "");
    strcpy(tests[ti++], "my dog is fin");
    strcpy(tests[ti++], "my dog is fine");
    strcpy(tests[ti++], "my dog is fin");
    strcpy(tests[ti++], "my dog is fine");
    strcpy(tests[ti++], "my dog m fine");
    strcpy(tests[ti++], "my dog mm fine");
    tests[ti++] = NULL;
    strcpy(tests[ti++], "CAsdfsAFDasdf");
    strcpy(tests[ti++], "a");
    strcpy(tests[ti++], "my dog is fine");
    for(int i = 0; i < TESTLEN; ++i){
        if(tests[i] == NULL)
            printf("(null)");
        else
            printf("\"%s\"",tests[i]);
        p = testc[i];
        if(p == 0)
            p = '_';
        printf(" '%c' -> %i\n", p, my_strrindex(tests[i], testc[i]));
    }
  for(int i = 0; i < TESTLEN; ++i){
    free(tests[i]);
  }
}
