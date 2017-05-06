#include "my.h"
#include <string.h>
#include <stdio.h>

void str_prnt(char * str){
  if(str == NULL)
    printf("(null)");
  else
    printf("\"%s\"",str);
}

void test(char * test){
  str_prnt(test);
  printf(" -> %i\n",my_atoi(test));
}

int main(){
  printf("TESTING my_atoi\n");
  
  test("0");
  test("10");
  test("3245");
  test("101");
  test("-1");
  test("-34");
  test("-10");
  test("1");
  test("10000");
  test("2147483647");
  test("-2147483648");
  test("1200000");
  test("1200001");
  test("asdfas0");
  test("a10");
  test("asd3245");
  test("d101");
  test("f-1");
  test("sd-asfd34");
  test("-ff10");
  test("1aspfaspdf9aps9fpa9snfpasd");
  test("asdf10000");
  test("asdfas2147483647o1ororr");
  test("sdfsd-sf--2147483648sdfsdf1");
  test("asdf1200000d");
  test("sdd1200001d");
  test(NULL);
  test("");
  
}
