#include <stdio.h>
#include <stdlib.h>

long long int getSum(int arr[], int index, int len){
	if (index == len) return 0;
	return arr[index] + getSum(arr, index + 1, len);
}

void test(){
	printf("\nSUM OF ARRAY\n");
	int arr[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	long long int sum = getSum(arr, 0, 10);
	printf("%d\n", sum);
}