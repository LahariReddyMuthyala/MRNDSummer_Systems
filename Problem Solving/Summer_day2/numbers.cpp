#include<stdio.h>
#include <stdlib.h>

void display(int *arr, int n){
	int i, leading = 0;
	for (i = 0; i < n - 1; i++){
	if (arr[i] != 0) leading = 1;
		if (leading)
			printf("%d", arr[i]);
	}
	printf(" ");
}
void printNumbers(int n, int *arr, int index){
	if (n-1 == index){
		display(arr, n);
		return;
	}
	int i;
	for (i = 0; i <=9; i++){
		arr[index] = i;
		printNumbers(n, arr, index + 1);
	}
}

void test_print(){
	printf("\nPRINT NUMBERS TILL 10^n-1");
	int n;
	printf("Enter n:");
	scanf_s("%d", &n);
	int *arr = (int*)calloc(n-1, sizeof(int));
	printNumbers(n, arr, 0);
}