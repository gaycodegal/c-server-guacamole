#include "my.h"
#include <string.h>
#include <stdio.h>

int main(){
    printf("Tests my_strlen\n");
    
    int testlen = 5;
    int maxstrlen = 16;
    char * tests[5];
    for(int i = 0; i < testlen; ++i){
        tests[i] = (char *)malloc(sizeof(char) * maxstrlen);
    }
    strcpy(tests[0], "my dog is fine");
    strcpy(tests[1], "");
    tests[2] = NULL;
    strcpy(tests[3], "CAsdfsAFDasdf");
    strcpy(tests[4], "a");
    for(int i = 0; i < testlen; ++i){
        if(tests[i] == NULL)
            printf("(null)");
        else
            printf("\"%s\"",tests[i]);
        printf(" -> %i\n", my_strlen(tests[i]));
    }
  
  for(int i = 0; i < testlen; ++i){
    free(tests[i]);
  }
}
