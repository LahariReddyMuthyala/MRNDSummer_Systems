#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct example{
	int friendCount;
	char name[8];
	char y;
	
};

void test_binaryFile(){
	int magicNums[6] = { 1920409673, 543519849, 543449442, 1701080931, 1767990304, 3045740 };
	//struct example *exam_temp=(struct example*)calloc(1, sizeof(struct example));
	//strcpy(exam_temp->name, "Lahari");
	//exam_temp->friendCount = 5;
	//exam_temp->y = 'a';
	FILE *fptr = fopen("test2.bin", "wb");
	/*int x = 0x31323334;
	int m = 4321;
	fwrite(exam_temp->name,8, 1, fptr);
	fprintf(fptr, "%s", exam_temp->name);
	fwrite(&m, sizeof(int), 1, fptr);
	fwrite(&x, sizeof(int), 1, fptr);
	fprintf(fptr, "%d", x);
	fprintf(fptr, "%d", m);
	printf("%d", sizeof(struct example));*/
	//fwrite(exam_temp, sizeof(exam_temp), 8, fptr);
	fwrite(magicNums, sizeof(int), 6, fptr);
	fclose(fptr);
}