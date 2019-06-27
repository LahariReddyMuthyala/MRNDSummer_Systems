#include <stdio.h>
#include <stdlib.h>

void display_perm_rep(int *arr, int n){
	int i;
	for (i = 0; i < n; i++){
		printf("%d", arr[i]);
	}
	printf(" ");
}

void getPermutationsRep(int arr[], int len, int *permutation, int valid, int index){
	int i, temp, t;
	int status[10] = { 0 };
	if (valid == len){
		display_perm_rep(permutation, len);
	}
	for (i = valid; i < len; i++){
		t = arr[i];
		if (status[t] == 1){
			continue;
		}
		else{
			status[t] = 1;
			permutation[index] = arr[i];

			temp = arr[i];
			arr[i] = arr[valid];
			arr[valid] = temp;

			getPermutationsRep(arr, len, permutation, valid + 1, index + 1);

			temp = arr[valid];
			arr[valid] = arr[i];
			arr[i] = temp;
		}

	}
}


void test_permutations_repetitive(){
	printf("\NPERMUTATIONS OF NUMBER WITH NON-UNIQUE DIGITS\n");
	int len, i;
	printf("Enter number of digits: ");
	scanf_s("%d", &len);
	int *arr = (int*)malloc(sizeof(int)*len);
	printf("\nEnter the elements: ");
	for (i = 0; i < len; i++){
		scanf_s("%d", &arr[i]);
	}
	int *perm = (int*)malloc(sizeof(int) * len);
	getPermutationsRep(arr, len, perm, 0, 0);
}