#include <stdio.h>
#include <stdlib.h>

long int fibo(long n, long int *arr, int size){
	if (n <=0) return 0;
	if (n == 1) return n;
	if (n < size && arr[n] == 0){
		arr[n-1] = fibo(n - 1, arr, size) + fibo(n - 2, arr, size);
		return arr[n-1];
	}
	else if (n < size && arr[n-1] != 0){
		return arr[n-1];
	}
	return fibo(n - 1, arr, size) + fibo(n - 2, arr, size);
}

void test_fibo(){
	printf("\nFIBONACCI\n");
	printf("Enter number :");
	int n;
	scanf("%d", &n);
	long int *memoArray = (long int*)calloc(n, sizeof(long int));
	long int fib = fibo(n, memoArray, n);
	printf("%ld\n", fib);
}