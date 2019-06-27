#include <stdio.h>
#include <stdlib.h>

void display_perm(int *arr, int n){
	int i;
	for (i = 0; i < n ; i++){
			printf("%d", arr[i]);
	}
	printf(" ");
}

void getPermutations(int arr[], int len, int *permutation, int valid, int index){
	int i, temp;
	if (valid == len){
		display_perm(permutation, len);
	}
	for (i = valid; i < len; i++){
		permutation[index] = arr[i];

		temp = arr[i];
		arr[i] = arr[valid];
		arr[valid] = temp;

		getPermutations(arr, len, permutation, valid + 1, index + 1);

		temp = arr[valid];
		arr[valid] = arr[i];
		arr[i] = temp;
	}
}


void test_permutations(){
	printf("\nPERMUTATIONS OF NUMBER WITH UNIQUE ELEMENTS\n");
	int len, i;
	printf("Enter number of digits: ");
	scanf_s("%d", &len);
	int *arr = (int*)malloc(sizeof(int)*len);
	printf("\nEnter the elements: ");
	for (i = 0; i < len; i++){
		scanf_s("%d", &arr[i]);
	}
	int *perm = (int*)malloc(sizeof(int) * len);
	getPermutations(arr, len, perm,0,0);
}