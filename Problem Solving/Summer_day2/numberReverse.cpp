#include<stdio.h>
#include <stdlib.h>

void display(char *arr, int n){
	int i, leading = 0;
	for (i = 0; i < n - 1; i++){
		if (arr[i] != 'O') leading = 1;
		if (leading)
			printf("%c", arr[i]);
	}
	printf(" ");
}

char reverse(int i){
	switch (i){
	case 0: return 'O';
	case 1: return 'I';
	case 2: return 'Z';
	case 3: return 'E';
	case 4: return 'h';
	case 5: return 'S';
	case 6: return '9';
	case 7: return 'L';
	case 8: return '8';
	case 9: return '6';
	}
}
void printNumbers(int n, char *arr, int index){
	if (n - 1 == index){
		display(arr, n);
		return;
	}
	int i;
	for (i = 0; i <= 9; i++){
		arr[index] = reverse(i);
		printNumbers(n, arr, index + 1);
	}
}

void test_printReverse(){
	printf("\n PRINT NUMBERS UPSIDE DOWN TILL 10^(n-1)\n");
	int n;
	printf("Enter n:");
	scanf_s("%d", &n);
	char *arr = (char*)calloc(n, sizeof(char));
	printNumbers(n, arr, 0);
}