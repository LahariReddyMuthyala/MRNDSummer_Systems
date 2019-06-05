#include <stdio.h>
#include <stdlib.h>

void test_generate(){
	FILE *fptr = fopen("numbers.txt", "w");
	int i;
	for (i = 10000; i > 0; i--){
		fprintf(fptr, "%d\n", i);
	}
	fclose(fptr);
}