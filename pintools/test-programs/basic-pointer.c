/*
 * basic-pointer.c
 *
 *  Created on: Jun 10, 2015
 *      Author: conor
 */

#include<stdio.h>
#include<stdlib.h>
int main() {
	for(int i=0; i<1000; i++) {
		int *number = (int*)malloc(sizeof(int));
		*number = i+2;
		printf("%d\n", *number);
	}
	return 0;
}

