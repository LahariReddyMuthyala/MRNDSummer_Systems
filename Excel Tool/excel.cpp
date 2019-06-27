#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum command{
	GET, SET, PRINT, IMPORT, EXPORT, SAVE, EXIT
};

enum operatorType{
	SUB=1, ADD, DIV, MUL, MOD, EXP, OPENBRACKET, CLOSEBRACKET
};

struct Node{
	int data;
	char *formula;
};

typedef struct Node node;
int operation(node ***matrix, char *operand1, char opera, char *operand2);


//create node of matrix
node* createNewNode(){
	node* newNode = (node*)calloc(1, sizeof(node));
	newNode->data = 0;
	newNode->formula = (char*)calloc(15,sizeof(char));
	return newNode;
}

//get value from GET command
char* getCommand(){
	int i=0;
	char c;
	char *command = (char*)calloc(35,sizeof(char));
	do{
		scanf_s("%c", &c);
		if ('a' <= c && c <= 'z') c = c - 32;
		if (c != '\n')
			command[i++] = c;
	} while (c != '\n');
	return command;
}

//checks if character is an operator
int isOperator(char ch){
	if (ch == '+') return ADD;
	if (ch == '-') return SUB;
	if (ch == '(') return OPENBRACKET;
	if (ch == ')') return CLOSEBRACKET;
	if (ch == '^') return EXP;
	if (ch == '%') return MOD;
	if (ch == '*') return MUL;
	if (ch == '/') return DIV;
	else
		return 0;
}

int exponent(int a, int b){
	if (b == 0) return 1;
	if (b == 1) return a;
	int p = exponent(a, b / 2);
	return p*p*exponent(a, b % 2);
}

int checkAssociativity(int a, int b){
	if ((a == SUB && b == ADD) || (b == SUB && a == ADD)) return 0;
	if ((a == MUL && b == DIV) || (a == MUL && b == MOD) || (a == DIV && b == MOD)) return 0;
	if ((b == MUL && a == DIV) || (b == MUL && a == MOD) || (b == DIV && a == MOD)) return 0;
	return 1;
}



//convert string to lowercase
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

//Returns the type of command
int getCommandIndex(char *command){
	char *buffer = (char*)calloc(8, sizeof(char));
	int i = 0;
	while (command[i] != ' ' && command[i] != '\t' && command[i]!='\0'){
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
		if (strcmp(lower(buffer), "exit") == 0){
			return EXIT;
		}
		if (strcmp(lower(buffer), "save") == 0){
			return SAVE;
		}
		
		printf("Invalid");
		return -1;

}

int checkCyclicDep(node ***matrix, char *attribute, int **visited, int count){
	if (attribute[0] == '\0' ) return 0;
	int i, col, row, k, visitedIter = 0;
	i = 0;
	while (attribute[i] == ' ' || attribute[i] == '\t') i++;
	col = attribute[i++] - 'A';
	while (attribute[i] == ' ' || attribute[i] == '\t') i++;
	row = 0;
	while (attribute[i] != '=' && attribute[i] != '\0' && attribute[i] != ' '){
		row *= 10;
		row += attribute[i] - '0';
		i++;
	}
	while (attribute[i] == ' ' || attribute[i] == '\t' || attribute[i] == '=') i++;
	if (attribute[i] == '\0'){
		printf("Invalid command\n");
		return -1;
	}
	visited[count][0] = row;
	visited[count++][1] = col;
	while (attribute[i] == ' ') i++;
	int flag = 0;
	int insertedValue = 0;
	while (attribute[i] != '\0'){
		if (attribute[i] == ' ' || attribute[i] == '\t') {
			i++;
			continue;
		}
		if (isdigit(attribute[i])){
			insertedValue *= 10;
			insertedValue += attribute[i] - '0';
			i++;
		}
		else{
			flag = 1;
			break;
		}
	}
	if (flag == 0){
		matrix[row - 1][col]->data = insertedValue;
		return 0;
	}
	else{
		int row1=0, col1=0;
		i = 0; visitedIter = 0;
		while (attribute[i] != '=') i++;
		while (attribute[i] == '=' || attribute[i] == ' ') i++;
		while (attribute[i] != '\0'){
			if (isOperator(attribute[i])){
				i++;
			}
			row1 = 0;
			col1 = 0;
			if (isdigit(attribute[i]) != 0){
				while (isdigit(attribute[i]) != 0) i++;
			}
			else if (isalpha(attribute[i]) != 0){
				k = 0;
				col1 = attribute[i++] - 'A';
				row1 = 0;
				while (attribute[i] != '=' && attribute[i] != '\0' && attribute[i] != ' ' && !isOperator(attribute[i])){
					row1 *= 10;
					row1 += attribute[i] - '0';
					i++;
				}
				for (visitedIter = 0; visitedIter < count; visitedIter++){
					if (visited[visitedIter][0] == row1 && visited[visitedIter][1] == col1) return 1;
				}
				return checkCyclicDep(matrix, matrix[row1 - 1][col1]->formula, visited, count);
			}
		}
		return 0;
	}


	            
}

//sets the value for SET command
int setValue(node ***matrix, char* command){
	int i = 0, j = 0;
	int row, col;
	char *attribute = (char*)calloc(40, sizeof(char));
	while (command[i] != ' ') i++;
	while (command[i] == ' ') i++;
	while (command[i] != '\0'){
		attribute[j++] = command[i++];
	}
	int **visited = (int**)calloc(20, sizeof(int*));
	for (i = 0; i < 20; i++) visited[i] = (int*)calloc(2, sizeof(int));

	int cycle = checkCyclicDep(matrix, attribute, visited, 0);
 	if (cycle == 1){
		printf("Invalid : Creates a cycle\n");
		return 0;
	}
	else if(cycle == 0){
		i = 0;
		while (attribute[i] == ' ' || attribute[i] == '\t') i++;
		col = attribute[i++] - 'A';
		while (attribute[i] == ' ' || attribute[i] == '\t') i++;
		row = 0;
		while (attribute[i] != '=' && attribute[i] != '\0' && attribute[i] != ' '){
			row *= 10;
			row += attribute[i] - '0';
			i++;
		}
		matrix[row - 1][col]->formula = attribute;
		return 1;
	}
	else if (cycle==-1){
		return -1;
	}
	
}


//parse each row for importing and get contents
int* parseRow(char *line){
	int *row = (int*)calloc(10, sizeof(int));
	int count = 0, value = 0, k=0;
	while (count < 10 && line[k]!='\0' && line[k]!='\n'){
		value = 0;
		while (line[k] != ',' && line[k] != '\0' && line[k]!='\n'){
			value = value * 10;
			value += line[k++] - '0';
		}
		k++;
		row[count++] = value;
	}
	return row;
}

char** parseMetaRow(char *line){
		char **metaRow = (char**)calloc(40, sizeof(char*));
		int i;
		for (i = 0; i < 10; i++) metaRow[i] = (char*)calloc(10, sizeof(char));
		int count = 0, value = 0, k = 0;
		while (count < 10 && line[k] != '\0' && line[k] != '\n'){
			value = 0;
			while (line[k] != ',' && line[k] != '\0' && line[k] != '\n'){
				metaRow[count][value++] = line[k++];
			}
			count++;
			k++;
		}
		return metaRow;
	}


//imports the file
void importFile(node ***matrix, char* file, char* metaFile){
	int i, j, k = 0;
	node *cell;
	char *line = (char*)calloc(1000, sizeof(char));
	int *row;
	char **metaRow;
	FILE *fptr = fopen(file, "r+");
	if (!fptr){
		printf("File does not exist\n");
		return;
	}
	for (i = 0; i < 10; i++){
		k = 0;
		fgets(line, 1000, fptr);
		row = parseRow(line);
		for (j = 0; j < 10; j++){
			cell = matrix[i][j];
			cell->data = row[j];
		}
	}
	fclose(fptr);

	fptr = fopen(metaFile, "r+");
	if (!fptr){
		for (i = 0; i < 10; i++){
			for (j = 0; j < 10; j++){
				cell = matrix[i][j];
				cell->formula = "\0";
			}
		}
	}
	else{
		for (i = 0; i < 10; i++){
			k = 0;
			fgets(line, 100, fptr);
			metaRow = parseMetaRow(line);
			for (j = 0; j < 10; j++){
				cell = matrix[i][j];
				cell->formula = metaRow[j];
			}
		}

	}

}


//Evaluates the value of a cell according to dependency
int expression(node*** matrix, int row, int col){
	node* currNode = matrix[row-1][col];
	int i, operatorStackPointer = -1, operandStackPointer = -1, k = 0;
	char **operandStack = (char**)calloc(200, sizeof(char*));
	char *operatorStack = (char*)calloc(30, sizeof(char));
	for (i = 0; i < 20; i++){
		operandStack[i] = (char*)calloc(30, sizeof(char));
	}
	
	char opera;
	char *operand1 = (char*)calloc(2, sizeof(char));
	char *operand2 = (char*)calloc(2, sizeof(char));
	char *value = (char*)calloc(10, sizeof(char));
	int result;
	if (currNode->formula[0] == '\0'){
		return currNode->data;
	}
	char *str = currNode->formula;
	i = 0;
	int row1 = 0, col1 = -1, row2 = 0, col2 = -1;
	while (str[i] != '=') i++;
	while (str[i] == ' ' || str[i] == '\t') i++;
	if (str[i] == '=') i++;
	while (str[i] == ' ' || str[i] == '\t') i++;

	while (str[i] != '\0'){
		if (str[i] == ' ' || str[i] == '\t') i++;
		if (isOperator(str[i])){
			if (str[i] == '(' || operatorStackPointer == -1 || (isOperator(str[i]) > isOperator(operatorStack[operatorStackPointer]) && str[i] != ')' && checkAssociativity(isOperator(str[i]), isOperator(operatorStack[operatorStackPointer])))) operatorStack[++operatorStackPointer] = str[i++];
			else{
				if (isOperator(str[i]) == CLOSEBRACKET){
					i++;
					while (operatorStack[operatorStackPointer] != '('){
						opera = operatorStack[operatorStackPointer--];
						k = 0;
						while (operandStack[operandStackPointer][k] != '\0') operand2[k] = operandStack[operandStackPointer][k++];
						operand2[k] = '\0';
						operandStackPointer--; k = 0;
						while (operandStack[operandStackPointer][k] != '\0') operand1[k] = operandStack[operandStackPointer][k++];
						operand1[k] = '\0';
						operandStackPointer--;
						result = operation(matrix, operand1, opera, operand2);
						sprintf(value, "%d", result);
						k = 0;
						operandStackPointer++;
						while (value[k] != '\0'){
							operandStack[operandStackPointer][k] = value[k++];
						}
						operandStack[operandStackPointer][k] = '\0';

					}
					operatorStackPointer--;
					continue;

				}
				else{
					if (operatorStack[operatorStackPointer] != '(' && operandStackPointer>1){
						opera = operatorStack[operatorStackPointer--];
						k = 0;
						while (operandStack[operandStackPointer][k] != '\0') operand2[k] = operandStack[operandStackPointer][k++];
						operand2[k] = '\0';
						operandStackPointer--; k = 0;
						while (operandStack[operandStackPointer][k] != '\0') operand1[k] = operandStack[operandStackPointer][k++];
						operand1[k] = '\0';
						operandStackPointer--;
						result = operation(matrix, operand1, opera, operand2);
						sprintf(value, "%d", result);
						k = 0;
						operandStackPointer++;
						while (value[k] != '\0'){
							operandStack[operandStackPointer][k] = value[k];
							k++;
						}
						operandStack[operandStackPointer][k] = '\0';
						//operatorStack[++operatorStackPointer] = str[i++];
						continue;
					}
					else{
						operatorStack[++operatorStackPointer] = str[i++];
					}
				}
				
			}
		}
		else if (isdigit(str[i]) != 0){
			k = 0;
			++operandStackPointer;
			while (isdigit(str[i]) != 0){
				operandStack[operandStackPointer][k++] = str[i++];
			}
			operandStack[operandStackPointer][k] = '\0';
		}
		else if (isalpha(str[i]) != 0){
			operandStack[++operandStackPointer][0] = str[i++];
			k = 1;
			while (isdigit(str[i]) != 0){
				operandStack[operandStackPointer][k++] = str[i++];
			}
			operandStack[operandStackPointer][++k] = '\0';
		}
	}

	while (operatorStackPointer > -1){
		opera = operatorStack[operatorStackPointer--];
		k = 0;
		while (operandStack[operandStackPointer][k] != '\0') operand2[k] = operandStack[operandStackPointer][k++];
		operand2[k] = '\0';
		operandStackPointer--; k = 0;
		while (operandStack[operandStackPointer][k] != '\0') operand1[k] = operandStack[operandStackPointer][k++];
		operand1[k] = '\0';
		operandStackPointer--;
		result = operation(matrix, operand1, opera, operand2);
		sprintf(value, "%d", result);
		operandStack[++operandStackPointer] = value;

	}
	
	return atoi(operandStack[0]);

}

int operation(node ***matrix, char *operand1, char opera, char *operand2){
	int i = 0, result, flag1 = 1, flag2 = 1;
	int row1 = 0, col1 = -1, row2 = 0, col2 = -1;
	if (isalpha(operand1[i]) != 0) col1 = operand1[i++] - 'A';
	if (operand1[i] == '-'){
		flag1 = -1;
		i++;
	}
	while (operand1[i] != '\0'){
		row1 *= 10;
		row1 += operand1[i] - '0';
		i++;
	}
	i = 0;
	if (isalpha(operand2[i]) != 0) col2 = operand2[i++] - 'A';
	if (operand2[i] == '-'){
		flag2 = -1;
	i++;
	}
	while (operand2[i] != '\0'){
		row2 *= 10;
		row2 += operand2[i] - '0';
		i++;
	}
	
	if (opera == '+') {
		if (col1 == -1 && col2 == -1)
			result = (flag1*row1) + (flag2*row2);
		else if (col1 == -1)
				result = flag1*row1 + expression(matrix, row2, col2);
		else if (col2 == -1)
			result = expression(matrix, row1, col1) + row2*flag2;
		else
			result = expression(matrix, row1, col1) + expression(matrix, row2, col2);
	}
	if (opera == '-'){
		if (col1 == -1 && col2 == -1)
			result = (flag1*row1) - (flag2*row2);
		else if (col1 == -1)
			result = flag1*row1 - expression(matrix, row2, col2);
		else if (col2 == -1)
			result = expression(matrix, row1, col1) - row2*flag2;
		else
			result = expression(matrix, row1, col1) - expression(matrix, row2, col2);
	}
	if (opera == '*'){
		 if (col1 == -1 && col2 == -1)
			result = (flag1*row1) * (flag2*row2);
		else if (col1 == -1)
			result = flag1*row1 * expression(matrix, row2, col2);
		else if (col2 == -1)
			result = expression(matrix, row1, col1) * row2*flag2;
		else
			result = expression(matrix, row1, col1) * expression(matrix, row2, col2);
	}
	if (opera == '/'){
		if (col1 == -1 && col2 == -1)
			result = (flag1*row1) / (flag2*row2);
		else if (col1 == -1)
			result = flag1*row1 / expression(matrix, row2, col2);
		else if (col2 == -1)
			result = expression(matrix, row1, col1) / row2*flag2;
		else
			result = expression(matrix, row1, col1) / expression(matrix, row2, col2);
	}
	if (opera == '%'){
		if (col1 == -1 && col2 == -1)
			result = (flag1*row1) % (flag2*row2);
		else if (col1 == -1)
			result = flag1*row1 % expression(matrix, row2, col2);
		else if (col2 == -1)
			result = expression(matrix, row1, col1) % row2*flag2;
		else
			result = expression(matrix, row1, col1) % expression(matrix, row2, col2);
	}
	if (opera == '^'){
		if (col1 == -1 && col2 == -1)
			result = exponent(row1, row2);
		else if (col1 == -1)
			result = exponent(row1, expression(matrix, row2, col2));
		else if (col2 == -1)
			result = exponent(expression(matrix, row1, col1), row2);
		else
			result = exponent(expression(matrix, row1, col1), expression(matrix, row2, col2));

	}
	return result;
}


//sets content of table according to dependency
void evaluate(node ***matrix){
	int i, j;
	for (i = 0; i < 10; i++){
		for (j = 0; j < 10; j++){
			matrix[i][j]->data = expression(matrix, i+1, j);
			
		}
	}
}

//Export data to file
void exportToFile(node ***matrix, char *file, char *metaFile){
	int l=0, i, j;
	FILE *fptr;
	evaluate(matrix);
	fptr = fopen(file, "w");
	for (i = 0; i < 10; i++){
		l = 0;
		for (j = 0; j < 10; j++){
			fprintf(fptr, "%d,", matrix[i][j]->data);
		}

		fprintf(fptr, "\n");
	}
	fclose(fptr);

	fptr = fopen(metaFile, "w");
	for (i = 0; i < 10; i++){
		l = 0;
		for (j = 0; j < 10; j++){
			fprintf(fptr, "%s,", matrix[i][j]->formula);
		}

		fprintf(fptr, "\n");
	}
	fclose(fptr);



}

//Returns the value of cell
int getValue(node ***matrix, char* command){
	int row = 0, col = 0, i = 0, j = 0;
	char *attribute = (char*)calloc(50, sizeof(char));
	while (command[i] != ' ') i++;
	while (command[i] == ' ') i++;
	while (command[i] != '\0'){
		attribute[j++] = command[i++];
	}

	i = 0;
	col = attribute[i++] - 'A';
	row = 0;
	while (attribute[i] != '\0' && attribute[i] != ' ' && isdigit(attribute[i])!=0){
		row *= 10;
		row += attribute[i] - '0';
		i++;
	}
	int result = expression(matrix, row, col);
	matrix[row - 1][col]->data = result;
	return result;

}

//Main driver
void operations(){
	int i, j, saveFlag = 0;

	char* metaFile = "metadata.csv";
	char* fileToSave = (char*)calloc(15, sizeof(char));
	char *buff = (char*)malloc(10 * sizeof(char));

	node ***matrix = (node***)calloc(10, sizeof(node**));
	for (i = 0; i < 10; i++){
		matrix[i] = (node**)calloc(10, sizeof(node*));
		for (j = 0; j < 10; j++){
			matrix[i][j] = createNewNode();
		}
	}

	do{
		printf("> ");
		char *command = getCommand();
		int commandIndex = getCommandIndex(command);
		if (commandIndex == -1)
			continue;
		if (commandIndex == GET){
			int value = getValue(matrix, command);
			printf("%d\n", value);
			continue;
		}
		if (commandIndex == SET){
			int value = setValue(matrix, command);
			if (value==-1)
				continue;
			continue;
		}

		if (commandIndex == PRINT){
			evaluate(matrix);
			for (i = 0; i < 10; i++){
				for (j = 0; j < 10; j++){
					printf("%d | ", matrix[i][j]->data);
				}
				printf("\n---------------------------------------\n");
			}
			continue;
		}

		if (commandIndex == IMPORT){
			sscanf(command, "%s %s", buff, fileToSave);
			saveFlag = 1;
			importFile(matrix, fileToSave, metaFile);
			continue;
		}

		if (commandIndex == SAVE){
			if (saveFlag){
				exportToFile(matrix, fileToSave, metaFile);
			}
			else{
				printf("File does not exist\n");
			}
		}

		if (commandIndex == EXIT){
			exit(0);
		}

		if (commandIndex == EXPORT){
			exportToFile(matrix, "temp.csv", metaFile);
		}

	} while (1);
}