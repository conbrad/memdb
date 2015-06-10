/*
 * basic-pointer.c
 *
 *  Created on: Jun 10, 2015
 *      Author: conor
 */

#include<stdio.h>

int main() {

	int *ptr_one;

	ptr_one = (int *)malloc(sizeof(int));

	if (ptr_one == 0) {
		printf("ERROR: Out of memory\n");
		return 1;
	}

	*ptr_one = 25;
	printf("%d\n", *ptr_one);

	free(ptr_one);

	return 0;
}

