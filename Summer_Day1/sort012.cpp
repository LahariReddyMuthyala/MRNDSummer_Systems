#include <stdio.h>
#include <stdlib.h>

void swap012(int *xp, int *yp)
{
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}

void display012(int *array, int len){
	int iter;
	for (iter = 0; iter < len; iter++){
		printf("%d ", array[iter]);
	}
}


void sort012(int *array, int len){
	int start = 0, end = len - 1;

	while (start < end)
	{
		while (array[start] == 0 && start < end)
			start++;

		while (array[end] > 0 && start < end)
			end--;

		if (start < end)
		{
			swap012(&array[start], &array[end]);
			start++;
			end--;
		}
	}
	end = len - 1;
	while (start < end)
	{
		while (array[start] == 1 && start < end)
			start++;

		while (array[end] > 1 && start < end)
			end--;

		if (start < end)
		{
			swap012(&array[start], &array[end]);
			start++;
			end--;
		}
	} 
}


void test_sort012(){
	printf("\nTHREE COLOR SORT - SORTING\n");
	printf("Enter length of array: ");
	int len, i;
	scanf("%d", &len);
	int *array = (int*)malloc(sizeof(int)*len);
	printf("Enter the array elements :\n");
	for (i = 0; i < len; i++){
		scanf("%d\n", &array[i]);
	}
	sort012(array, len);
	display012(array, len);
}