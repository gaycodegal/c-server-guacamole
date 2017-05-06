#include "my.h"
#include <stdlib.h> 

/* I pledge my honor that I have abided
by the Stevens Honor System - Steph Oro */

void my_int(int x){
	int od = 1, ev = 1, m = 1;
	if(x == 0){
		my_char('0');
		return;
	}
	if(x < 0){
		m = -1;
		my_char('-');
	}
	while(x > 9 || x < -9){
		od = od*10 + (x%10)*m;
		x = x/10;
		ev = ev*10 + (x%10)*m;
		x = x/10;
	}
	if(x)
		my_char(x*m + '0');
	while(od > 1 || ev > 1){
		my_char(ev%10 + '0');
		ev = ev / 10;
		my_char(od%10 + '0');
		od = od / 10;
	}
}
