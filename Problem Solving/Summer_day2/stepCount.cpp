#include <stdio.h>
#include <stdlib.h>

int stepCount(int n, int k){
	if (n == 0 || n==1)
		return 1;
	if (n < 0) return 0;
	int i, counts=0;
	for (i = 1; i <= k; i++){
		counts = counts + stepCount(n - i, k);
	}
	return counts;
}

void test_stepCount(){
	printf("\nNUMBER OF WAYS TO TAKE N STEPS WITH 1-K VALID STEPS\n");
	int n, k;
	printf("Enter number of steps: ");
	scanf_s("%d", &n);
	printf("\nEnter possible steps: ");
	scanf_s("%d", &k);
	int possibleWays = stepCount(n, k);
	printf("%d ", possibleWays);
}