#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* compress(char *string){
	int len = strlen(string);
	int reverse = 0;
	if (len == 1) return string;
	int slow = 0, fast = 1, count = 1, k = 0;
	char *compressed = (char*)calloc(len, sizeof(char));
	while (fast < len){
		if (string[slow] != string[fast]){
			compressed[k++] = string[slow];
			if (count > 1){
				compressed[k++] = count + '0';
			}
			count = 0;
		}
		slow++;
		fast++;
		count++;
	}
	compressed[k++] = string[slow];
	if (count > 1){
		reverse = 0;
		while (count > 0){
			reverse *= 10;
			reverse += count % 10;
			count /= 10;
		}
		while (reverse>0){
			compressed[k++] = reverse % 10 + '0';
			reverse /= 10;
		}
	}
	
	return compressed;
}

void test_compress(){
	printf("\nSTRING COMPRESSION\n");
	char *string = (char*)calloc(50, sizeof(char));
	while (1){
		printf("Enter the string: ");
		scanf("%s", string);
		printf("compressed string : %s\n", compress(string));
	}
}