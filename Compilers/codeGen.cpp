#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct symbolTable{
	char symbol[7];
	int address, size;
};

struct labelTable{
	char name[10];
	int address;
};

struct intermediateTable{
	int lineNo;
	int opCode;
	int parameters[4];
};

typedef struct symbolTable symbolTable;
typedef struct labelTable labelTable;
typedef struct intermediateTable interTable;

enum opcode{
	MOVRM=1, MOVMR, ADD, SUB, MUL, JUMP, IF, EQ, LT, GT, LTEQ, GTEQ, PRINT, READ, DATA, CONST, ELSE, ENDIF
};

enum reg{
	AX, BX, CX, DX, EX, FX, GX, HX
};

bool isVariable(char *str){
	if (strlen(str) == 1) return true;
	return false;
}

int isReg(char *str){
	if (strcmp(str, "AX") == 0) return AX;
	if (strcmp(str, "BX") == 0) return BX;
	if (strcmp(str, "CX") == 0) return CX;
	if (strcmp(str, "DX") == 0) return DX;
	if (strcmp(str, "EX") == 0) return EX;
	if (strcmp(str, "FX") == 0) return FX; 
	if (strcmp(str, "GX") == 0) return GX; 
	if(strcmp(str, "HX") == 0) return HX;
	return -1;
}


int getOpcode(char *instr, char *operands){
	char *oper = (char*)calloc(7, sizeof(char));
	int i, j;
	if (strcmp(instr, "MOV") == 0){
		i = 0;
		while (operands[i] != ' ' && operands[i] != ','){
			oper[i] = operands[i];
			i++;
		}
		oper[i] = '\0';
		if (isReg(oper)) return MOVMR;
		if (isVariable(oper)) return MOVRM;
	}
	else if (strcmp(instr, "ADD") == 0) return ADD;
	else if (strcmp(instr, "SUB") == 0) return SUB;
	else if (strcmp(instr, "MUL") == 0) return MUL;
	else if (strcmp(instr, "JUMP") == 0) return JUMP;
	else if (strcmp(instr, "IF") == 0) return IF;
	else if (strcmp(instr, "EQ") == 0) return EQ;
	else if (strcmp(instr, "GT") == 0) return GT;
	else if (strcmp(instr, "LT") == 0) return LT;
	else if (strcmp(instr, "LTEQ") == 0) return LTEQ;
	else if (strcmp(instr, "GTEQ") == 0) return GTEQ;
	else if (strcmp(instr, "PRINT") == 0) return PRINT;
	else if (strcmp(instr, "READ") == 0) return READ;
	else if (strcmp(instr, "DATA") == 0) return DATA;
	else if (strcmp(instr, "CONST") == 0) return CONST;
	else if (strcmp(instr, "ELSE") == 0) return ELSE;
	else if (strcmp(instr, "ENDIF") == 0) return ENDIF;
	return 0;

	}

int hasArray(char *buff){
	int i=0, offset = 0;
	while (buff[i] != '[' && buff[i] != '\0' && buff[i] != '\r') i++;
	if (buff[i] == '\0' || buff[i] == '\r') return -1;
	i++;
	while (buff[i] != ']'){
		offset *= 10;
		offset += buff[i] - '0';
		i++;
	}
	return offset;
}


void processArith(interTable *table, int *lineNum, char *parameters, symbolTable *symbolTab, int *symbolCount, int *tableCount){
	char *buff = (char*)calloc(4, sizeof(char));
	int paramIter = 0, i;
	for (i = 0; i < 4; i++){
		table[*(tableCount)].parameters[i] = -1;
	}
	int iter = 0, tempIter = 0;
	while (parameters[iter] != '\0' && parameters[iter] != '\r')
	{
		if (parameters[iter] == ',' || parameters[iter] == ' '){
			iter++;
		}
		else{
			tempIter = 0;
			while (parameters[iter] != ',' && parameters[iter] != '\0' && parameters[iter] != '\r' && parameters[iter] != ' ')
			{
				buff[tempIter++] = parameters[iter++];
			}
			buff[tempIter] = '\0';

			if (getOpcode(buff, "")){
				table[*(tableCount)].parameters[paramIter++] = getOpcode(buff, "");
			}

			else if (hasArray(buff)>-1)
			{
				tempIter = 0;
				while (buff[tempIter] != '[' && buff[tempIter] != ']')tempIter++;
				buff[tempIter] = '\0';
				int offset = hasArray(buff);
				for (i = 0; i < *(symbolCount); i++)
				{
					if (strcmp(symbolTab[i].symbol, buff) == 0)
					{
						table[*(tableCount)].parameters[paramIter++] = symbolTab[i].address + offset;
					}
				}
			}

			else if (isReg(buff) != -1)
			{
				table[*(tableCount)].parameters[paramIter++] = isReg(buff);
			}
			else{
				for (i = 0; i < *(symbolCount); i++)
				{
					if (strcmp(symbolTab[i].symbol, buff) == 0)
					{
						table[*(tableCount)].parameters[paramIter++] = symbolTab[i].address;
					}
				}

			}

		}

	}
	/*if (isReg(buff) != -1)
	{
		table[*(tableCount)].parameters[paramIter++] = isReg(buff);
	}
	else{
		for (i = 0; i < *(symbolCount); i++)
		{
			if (strcmp(symbolTab[i].symbol, buff) == 0)
			{
				table[*(tableCount)].parameters[paramIter] = symbolTab[i].address;
			}
		}

	}*/
}

void insertIntoLabelTab(labelTable *labelTab, int *labelCount, char* line, int *lineNum){
	char *buff = (char*)calloc(10, sizeof(char));
	int iter = 0;
	while (line[iter] != ':' && line[iter] != '\0' && line[iter] != '\r' && line[iter] != ' '){
		buff[iter] = line[iter];
		iter++;
	}
	line[iter] = '\0';
	strcpy(labelTab[*(labelCount)].name, buff);
	labelTab[*(labelCount)].address = *(lineNum);
	*(labelCount) += 1;
}

void processLine(char *line, interTable *table, symbolTable *symbolTab, labelTable *labelTab, int *lineNum, int *symbolCount, int *memoryPointer, int *tableCount, int *labelCount, int *stackPointer, int *stack){
	if (strcmp(line, "START:\r") == 0 || strcmp(line, "END") == 0) return;
	if (line[strlen(line) - 2] == ':'){
		insertIntoLabelTab(labelTab, labelCount, line, lineNum);
		*(lineNum) += 1;
		return;
	}

	char *instruction = (char*)calloc(10, sizeof(char));
	char *parameters = (char*)calloc(30, sizeof(char));
	char *symbol = (char*)calloc(7, sizeof(char));
	int value = 0;
	int iter = 0, tempIter = 0;
	while (line[iter] != ' ' && line[iter]!='\r'){
			instruction[iter] = line[iter++];
	}
	iter++; tempIter = 0;
	while (line[iter] != '\0' && line[iter] != '\r') parameters[tempIter++] = line[iter++];
	parameters[tempIter] = '\0';
	int opCode = getOpcode(instruction, parameters);
	if (opCode == DATA || opCode == CONST){
		int symbolIndex = *(symbolCount);
		*(symbolCount) = symbolIndex + 1;
		int memPointer = *(memoryPointer);
		symbolTab[symbolIndex].address = *(memoryPointer);
		iter = 0; tempIter = 0;
		while (parameters[iter] != '[' && parameters[iter] != '\0' && parameters[iter] != '\r' && parameters[iter] != '='){
			symbol[tempIter++] = parameters[iter++];
		}
		symbol[tempIter] = '\0';
		strcpy(symbolTab[symbolIndex].symbol, symbol);
		if (parameters[iter] == '\0' || parameters[iter] == '\r'){
			symbolTab[symbolIndex].size = 1;
			*(memoryPointer) = memPointer + 1;
		}
		if (parameters[iter] == '['){
			iter++;
			value = 0;
			while (parameters[iter] != ']'){
				value *= 10;
				value += parameters[iter++] - '0';
			}
			symbolTab[symbolIndex].size = value;
			*(memoryPointer) = memPointer + value;
		}
		if (opCode == CONST){
			if (parameters[iter] == '=') iter++;
			value = 0;
			while (parameters[iter] != '\r' && parameters[iter] !='\0'){
				value *= 10;
				value += parameters[iter++] - '0';
			}
			symbolTab[symbolIndex].size = 0;
		}
	}
	else{
		*(lineNum) = *(lineNum)+1;
	 if (opCode == ADD || opCode == SUB || opCode == MUL || opCode == PRINT || opCode == MOVMR || opCode == MOVRM || opCode == READ)
	 {
		table[*(tableCount)].lineNo = *(lineNum);
		table[*(tableCount)].opCode = opCode;
		processArith(table, lineNum, parameters, symbolTab, symbolCount, tableCount);
		*(tableCount) = *(tableCount)+1;
	}
	 else if (opCode == JUMP){
		 table[*(tableCount)].lineNo = *(lineNum);
		 table[*(tableCount)].opCode = opCode;
		 for (iter = 0; iter < *(symbolCount); iter++){
			 if (strcpy(labelTab[iter].name, parameters) == 0) break;

		 }
		 table[*(tableCount)].parameters[0] = labelTab[iter].address;
		 *(tableCount) = *(tableCount) + 1;
	}
	 else if (opCode == IF){
		 table[*(tableCount)].lineNo = *(lineNum);
		 table[*(tableCount)].opCode = opCode;
		 processArith(table, lineNum, parameters, symbolTab, symbolCount, tableCount);
		 stack[++*(stackPointer)] = *(lineNum);
		 *(tableCount) = *(tableCount)+1;
	 }
	 else if (opCode == ELSE){
		 table[*(tableCount)].lineNo = *(lineNum);
		 table[*(tableCount)].opCode = JUMP;
		 int ifLine = stack[*(stackPointer)];
		 stack[*(stackPointer)] = *(lineNum);
		 table[ifLine - 1].parameters[3] = *(lineNum)+1;
		 *(tableCount) = *(tableCount)+1;
	 }
	 else if (opCode == ENDIF){
		 int ifLine = stack[*(stackPointer)];
		 *(stackPointer) -= 1;
		 table[ifLine - 1].parameters[3] = *(lineNum)+1;
	 }
	}


}

void writeToFile(interTable *table, int *tableCount, char *outputFile){
	FILE *fptr = fopen(outputFile, "w+");
	int i, j;
	for (i = 0; i < *(tableCount); i++){
		fprintf(fptr, "%02d %02d", table[i].lineNo, table[i].opCode);
		for (j = 0; j < 4; j++){
			if (table[i].parameters[j] > -1) 
				fprintf(fptr, " %02d", table[i].parameters[j]);
		}
		fprintf(fptr, "\n");
	}

	fclose(fptr);
}

void generateSymbolTable(interTable *table,symbolTable *symbolTab, labelTable *labelTab, char *inputText, char* outputFile){
	int lineNum = 0; int memoryPointer = 0, symbolCount = 0, tableCount = 0, labelCount = 0, stackPointer = -1;
	int *stack = (int*)calloc(20, sizeof(int));
	char *line = (char*)calloc(40, sizeof(char));
	int iter = 0, lineIter = 0;
	while(inputText[iter] != '\0'){
		lineIter = 0;
		while (inputText[iter] != '\n' && inputText[iter] != '\0') line[lineIter++] = inputText[iter++];
		line[lineIter] = '\0';
		processLine(line, table, symbolTab, labelTab, &lineNum, &symbolCount, &memoryPointer, &tableCount, &labelCount, &stackPointer, stack);
		iter++;
	}
	writeToFile(table, &tableCount, outputFile);
	

}


void readInputFile(char *inputText, char *inputF, long int fileSize){
	FILE *fptr = fopen(inputF, "rb");
	fread(inputText, fileSize, 1, fptr);
	int k = 0;
	inputText[fileSize] = '\0';
	fclose(fptr);
}


long int getFileSize(char *fileName){
	FILE* fp = fopen(fileName, "rb");
	if (fp == NULL) {
		printf("File Not Found!\n");
		return -1;
	}
	fseek(fp, 0L, SEEK_END);
	long int size = ftell(fp);
	fclose(fp);
	return size;

}

void test_codeGen(){
	char *inputFile = (char *)calloc(10, sizeof(char));
	char *outputFile = (char*)calloc(10, sizeof(char));
	char *input = (char*)calloc(40, sizeof(char));
	char temp[8];
	printf("compile <source.txt> <destination.txt>\n");
	printf("> ");
	scanf("%s", input);
	scanf("%s", inputFile);
	scanf("%s", outputFile);
	long int fileSize = getFileSize(inputFile);
	char *inputText = (char*)calloc(fileSize+10,sizeof(char));
	readInputFile(inputText, inputFile, fileSize);
	interTable *table = (interTable*)malloc(1000 * sizeof(interTable));
	symbolTable *symbolTab = (symbolTable*)malloc(100 * sizeof(symbolTable));
	labelTable *labelTab = (labelTable*)malloc(100 * sizeof(labelTable));
	generateSymbolTable(table, symbolTab, labelTab, inputText, outputFile);

}


