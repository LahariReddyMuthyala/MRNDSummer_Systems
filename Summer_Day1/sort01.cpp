#include<stdio.h>
#include<stdlib.h>

void display(int *array, int len){
	int i;
	for (i = 0; i < len; i++){
		printf("%d ", array[i]);
	}
}

void sort01(int *binArray, int len){
	int count = 0, i; // Counts the no of zeros in arr 

	for (i = 0; i < len; i++) {
		if (binArray[i] == 0)
			count++;
	}

	for (i = 0; i < count; i++)
		binArray[i] = 0;

	for (i = count; i < len; i++)
		binArray[i] = 1;


}

void sort02(int *binArray, int len){
	int start = 0, end = len - 1;

	while (start < end)
	{
		while (binArray[start] == 0 && start < end)
			start++;

		while (binArray[end] == 1 && start < end)
			end--;

		if (start < end)
		{
			binArray[start] = 0;
			binArray[end] = 1;
			start++;
			end--;
		}
	}
}

void swap(int *xp, int *yp)
{
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}

void sort03(int *binArray, int len){
		int i, j;
		for (i = 0; i < len-1; i++){   
			for (j = 0; j < len - i - 1; j++)
				if (binArray[j] > binArray[j + 1])
					swap(&binArray[j], &binArray[j + 1]);
	}



}
void testApproach_1(){
	printf("\nTWO COLOR SORT - COUNTING 0'S AND 1'S\n");
	printf("Enter length of array: ");
	int len, i;
	scanf_s("%d", &len);
	int *binArray = (int*)malloc(sizeof(int)*len);
	printf("Enter the array elements :\n");
	for (i = 0; i < len; i++){
		scanf_s("%d", &binArray[i]);
	}
	display(binArray, len);
	sort01(binArray, len);
	printf("\n");
	display(binArray, len);
	printf("\n");
	system("pause");
	//scanf_s("%s");
}

void testApproach_2(){
	printf("\nTWO COLOR SORT - TWO POINTER METHOD\n");
	printf("Enter length of array: ");
	int len, i;
	scanf_s("%d", &len);
	int *binArray = (int*)malloc(sizeof(int)*len);
	printf("Enter the array elements :\n");
	for (i = 0; i < len; i++){
		scanf_s("%d\n", &binArray[i]);
	}
	display(binArray, len);
	sort02(binArray, len);
	printf("\n");
	display(binArray, len);
	printf("\n");
	system("pause");
	//scanf_s("%s");
}

void testApproach_3(){
	printf("\nTWO COLOR SORT - SORTING\n");
	printf("Enter length of array: ");
	int len, i;
	scanf_s("%d", &len);
	int *binArray = (int*)malloc(sizeof(int)*len);
	printf("Enter the array elements :\n");
	for (i = 0; i < len; i++){
		scanf_s("%d\n", &binArray[i]);
	}
	display(binArray, len);
	sort03(binArray, len);
	printf("\n");
	display(binArray, len);
	printf("\n");


}