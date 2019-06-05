#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum command{
	GET, SET, PRINT, IMPORT, EXPORT
};

struct Node{
	int data;
	char *formula;
};

typedef struct Node node;

char* getCommand(){
	int i = 0;
	char c;
	char *command = (char*)calloc(20, sizeof(char));
	do{
		scanf_s("%c", &c);
		if (c != '\n')
			command[i++] = c;
	} while (c != '\n');
	return command;
}

FILE* getNewFile(){
	FILE *fptr = fopen("temp.csv", "w+");
	if (!fptr){
		printf("Error opening file");
		return NULL;
	}
	return fptr;

}

char* lower(char *string){
	int c = 0;
	while (string[c] != '\0') {
		if (string[c] >= 'A' && string[c] <= 'Z') {
			string[c] = string[c] + 32;
		}
		c++;
	}
	return string;
}

int getCommandIndex(char *command){
	char *buffer = (char*)calloc(8, sizeof(char));
	int i = 0;
	while (command[i] != ' ' && command[i] != '\t' && command[i] != '\0'){
		buffer[i] = command[i];
		i++;
	}
	if (strcmp(lower(buffer), "get") == 0){
		return GET;
	}
	if (strcmp(lower(buffer), "set") == 0){
		return SET;
	}
	if (strcmp(lower(buffer), "print") == 0){
		return PRINT;
	}
	if (strcmp(lower(buffer), "import") == 0){
		return IMPORT;
	}
	if (strcmp(lower(buffer), "export") == 0){
		return EXPORT;
	}

	printf("Invalid");
	return -1;

}

void setValue(int **matrix, char* command){
	int i = 0;
	int row, col;
	while (command[i] != ' ') i++;
	while (command[i] == ' ' || command[i] == '\t') i++;
	col = command[i++] - 'A';
	row = 0;
	while (command[i] != '='){
		row *= 10;
		row += command[i] - '0';
		i++;
	}
	while (command[i] == ' ' || command[i] == '=') i++;
	int insertedValue = 0;
	while (command[i] != '\0'){
		insertedValue *= 10;
		insertedValue += command[i] - '0';
		i++;
	}
	matrix[row - 1][col] = insertedValue;
	return;
}

void importFile(int **matrix, char* command){
	int i, j, k = 0;
	char *file = (char*)calloc(15, sizeof(char));
	char *comm = (char*)calloc(10, sizeof(char));
	char *line = (char*)calloc(45, sizeof(char));
	sscanf(command, "%s %s", comm, file);
	FILE *fptr = fopen(file, "w");
	for (i = 0; i < 10; i++){
		k = 0;
		fgets(line, 100, fptr);
		printf("%s", line);
		for (j = 0; j < 10; j++){
			matrix[i][j] = line[k];
			k += 2;
		}
	}
	fclose(fptr);

}



void operations(){
	FILE *fptr = getNewFile();
	int i, j, row, col, l;
	char* buffer = (char*)calloc(5, sizeof(char));
	int **matrix = (int**)calloc(10, sizeof(int*));
	for (i = 0; i < 10; i++){
		matrix[i] = (int*)calloc(10, sizeof(int));
	}
	do{
		printf("> ");
		char *command = getCommand();
		int commandIndex = getCommandIndex(command);
		if (commandIndex == -1)
			continue;
		if (commandIndex == GET){
			i = 0;
			while (command[i] != ' ' && command[i] != '\t'){
				i++;
			}
			i++;
			j = 0;
			while (command[i] != '\0'){
				buffer[j++] = command[i++];
			}
			buffer[j] = '\0';
			j = 0;
			row = 0;
			if (strlen(buffer) == 2){
				if ('a' <= buffer[0] && buffer[0] <= 'z')
					col = buffer[0] - 'a';
				else
					col = buffer[0] - 'A';
				row = buffer[1] - '0';
			}
			else{
				if ('a' <= buffer[0] && buffer[0] <= 'z')
					col = buffer[0] - 'a';
				else
					col = buffer[0] - 'A';
				row = 9;
			}
			printf("%d\n", matrix[row - 1][col]);
			continue;
		}
		if (commandIndex == SET){
			setValue(matrix, command);
			continue;
		}

		if (commandIndex == PRINT){
			for (i = 0; i < 10; i++){
				for (j = 0; j < 10; j++){
					printf("%d | ", matrix[i][j]);
				}
				printf("\n-----------------------------\n");
			}
			continue;
		}

		if (commandIndex == IMPORT){
			importFile(matrix, command);
			continue;
		}

		if (commandIndex == EXPORT){
			fclose(fptr);
			fptr = fopen("data.csv", "w");
			l = 0;
			for (i = 0; i < 10; i++){
				l = 0;
				for (j = 0; j < 10; j++){
					fprintf(fptr, "%d,", matrix[i][j]);
				}

				fprintf(fptr, "\n");
			}
			fclose(fptr);
		}

	} while (1);

}