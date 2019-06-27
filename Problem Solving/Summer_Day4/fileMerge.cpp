#include <stdio.h>
#include <stdlib.h>
#define AUXFILE1 "temp1.txt"
#define AUXFILE2 "temp2.txt"
#define AUXFILE3 "temp3.txt"
#define ARRAYLIM 16000

void copyContent(char *file1, char *file2){
	FILE *fptr1 = fopen(file1, "w+");
	FILE *fptr2 = fopen(file2, "r+");
	char *buff = (char*)calloc(10, sizeof(char));
	int k = 0;
	while (k != EOF){
		k = fscanf(fptr2, "%s", buff);
		if (k != EOF)
			fprintf(fptr1, "%s\n", buff);
	}
	fclose(fptr1);
	fclose(fptr2);
}

int stringToInt(char *str){
	int val = 0;
	int i = 0;
	while (str[i] != '\0'){
		val *= 10;
		val += str[i] - '0';
		i++;
	}
	return val;
}

void mergeFiles(char *file1, char* file2, char* file3){
	FILE *fptr1 = fopen(file1, "r+");
	FILE *fptr2 = fopen(file2, "r+");
	FILE *fptr3 = fopen(file3, "w+");
	char *strNum1 = (char*)calloc(10,sizeof(char));
	char *strNum2 = (char*)calloc(10, sizeof(char));
	int k1 = 0, k2 = 0, a, b, flag1 = 0, flag2 = 0;
	k1 = fscanf(fptr1, "%s", strNum1);
	k2 = fscanf(fptr2, "%s", strNum2);
	a = stringToInt(strNum1);
	b = stringToInt(strNum2);
	while (k1 != EOF && k2 != EOF){
		if (flag1){
			k1 = fscanf(fptr1, "%s", strNum1);
			a = stringToInt(strNum1);
		}
		else if (flag2){
			k2 = fscanf(fptr2, "%s", strNum2);
			b = stringToInt(strNum2);
		}
		if (a > b && k2!=EOF){
			fprintf(fptr3, "%d\n", b);
			flag2 = 1;
			flag1 = 0;
		}
		else if (a<b && k1!=EOF){
			fprintf(fptr3, "%d\n", a);
			flag1 = 1;
			flag2 = 0;
		}
	}
	if (k1 != EOF){
		fprintf(fptr3, "%d\n", a);
		while (k1 != EOF){
			k1 = fscanf(fptr1, "%s", strNum1);
			a = stringToInt(strNum1);
			if (k1!=EOF)
				fprintf(fptr3, "%d\n", a);
		}
	}
	if (k2 != EOF){
		fprintf(fptr3, "%d\n", b);
		while (k2 != EOF){
			k2 = fscanf(fptr2, "%s", strNum2);
			b = stringToInt(strNum2);
			if (k2!=EOF)
				fprintf(fptr3, "%d\n", b);
		}
	}
	fclose(fptr3);
}


void merge(int arr[], int l, int m, int r)
{
	int i, j, k;
	int n1 = m - l + 1;
	int n2 = r - m;
	int *L = (int*)calloc(n1, sizeof(int));
	int *R = (int*)calloc(n2, sizeof(int));
	

	for (i = 0; i < n1; i++)
		L[i] = arr[l + i];
	for (j = 0; j < n2; j++)
		R[j] = arr[m + 1 + j];

	
	i = 0; 
	j = 0;
	k = l;
	while (i < n1 && j < n2)
	{
		if (L[i] <= R[j])
		{
			arr[k] = L[i];
			i++;
		}
		else
		{
			arr[k] = R[j];
			j++;
		}
		k++;
	}

	while (i < n1)
	{
		arr[k] = L[i];
		i++;
		k++;
	}

	while (j < n2)
	{
		arr[k] = R[j];
		j++;
		k++;
	}
	free(L);
	free(R);
}


void mergeSort(int *arr, int l, int r)
{
	if (l < r)
	{
		int m = l + (r - l) / 2;
		mergeSort(arr, l, m);
		mergeSort(arr, m + 1, r);
		merge(arr, l, m, r);
	}
}


void sortNumbers(char *fileName){
	char *strNum = (char*)calloc(10, sizeof(char));
	FILE *fptr = fopen(fileName, "r+");
	int *arrayNumbers = (int*)calloc(ARRAYLIM, sizeof(int));
	int i=0, k=0;
	while (i < ARRAYLIM && k != EOF){
		k = fscanf(fptr, "%s", strNum);
		if (k!=EOF)
			arrayNumbers[i++] = stringToInt(strNum);
	}
	int len = i;
	mergeSort(arrayNumbers,0, len-1);
	i = 0;
	fseek(fptr, 0, SEEK_SET);
	while (i < len){
		fprintf(fptr, "%d\n", arrayNumbers[i]);
		i++;
	}
	fclose(fptr);
	free(arrayNumbers);
	free(strNum);

}

void loadData(FILE *fptr1, char *file2){
	FILE *fptr2 = fopen(file2, "w+");
	int k = 0;
	int ch=0;
	char *strNum = (char*)calloc(10, sizeof(char));
	while (k < ARRAYLIM && ch!=EOF){
		ch = fscanf(fptr1, "%s", strNum);
		if (ch!=EOF)
			fprintf(fptr2, "%s\n", strNum);
		k++;
	}
	fclose(fptr2);
}


void testSortFromFile(){
	FILE *inputFile;
	inputFile = fopen("input.txt", "r+" );
	if (!inputFile){
		printf("Error opening input file");
		exit(0);
	}
	loadData(inputFile, AUXFILE1);
	loadData(inputFile, AUXFILE2);
	sortNumbers(AUXFILE1);
	sortNumbers(AUXFILE2);
	mergeFiles(AUXFILE1, AUXFILE2, AUXFILE3);
	copyContent(AUXFILE2, AUXFILE3);
	while (!feof(inputFile)){
		loadData(inputFile, AUXFILE1);
		sortNumbers(AUXFILE1);
		mergeFiles(AUXFILE1, AUXFILE2, AUXFILE3);
		copyContent(AUXFILE2, AUXFILE3);
	}
	fclose(inputFile);
}