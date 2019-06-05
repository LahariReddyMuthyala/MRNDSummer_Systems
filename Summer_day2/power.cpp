#include <stdio.h>
#include <stdlib.h>

int power(int a, int n){
	if (n == 0) return 1;
	if (n == 1) return a;
	int p = power(a, n / 2);
	return p*p*power(a, n % 2);
}

void test_power(){
	printf("\na^n\n");
	int a, n;
	printf("Enter value of 'a': ");
	scanf_s("%d", &a);
	printf("\nEnter value of 'k'");
	scanf_s("%d", &n);
	printf("%d ^ %d = %d", a, n, power(a, n));
}
