#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BUFFSIZE 16*1024
#define BLOCKNUM 6400
#define BLOCKSIZE (1024*16)
#define TEMPBLOCKNUM 32
#define DISKFILENAME "db.hdd"

enum command{
	GET=1, SET, DELETE, LS
};

struct file{
	char file_name[20];
	unsigned int stack_length, no_of_blocks, file_length;
};

struct meta{
	int magic;
	struct file file_pointers[32];
	int no_of_files;
	bool blocks[6400];
};

typedef struct meta meta;
typedef struct file file;

int getCommand(char *command){
	int i = 0;
	char *buff = (char*)calloc(10, sizeof(char));
	while (command[i] != ' ' && command[i]!='\0'){
		if ('a' <= command[i] && command[i] <= 'z') 
			command[i] -= 32;
		buff[i] = command[i];
		i++;
	}
	if (strcmp(buff, "GET") == 0) return GET;
	if (strcmp(buff, "SET") == 0) return SET;
	if (strcmp(buff, "LS") == 0) return LS;
	if (strcmp(buff, "DELETE") == 0) return DELETE;
}

void format(void *buffer){
	FILE *diskFile = fopen(DISKFILENAME, "rb+");
	meta metadata;
	metadata.magic = 0x444E524D;
	metadata.no_of_files = 0;
	int i;
	metadata.blocks[0] = false;
	for (i = 1; i < BLOCKNUM; i++){
		metadata.blocks[i] = true;
	}
	/*for (i = 0; i < 32; i++){
	metadata.file_pointers[i] = 0;
	}*/
	memcpy(buffer, &metadata, sizeof(metadata));
	fwrite(buffer, sizeof(meta), 1, diskFile);
	fclose(diskFile);

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

meta get_meta(void *buffer){
	meta metadata;
	FILE *diskFile = fopen("db.hdd", "rb+");
	fread(buffer, sizeof(meta), 1, diskFile);
	memcpy(&metadata, buffer, sizeof(meta));
	if (metadata.magic != 0x444E524D){
		printf("Disk is corrupted\n");
		format(buffer);
	}
	fclose(diskFile);
	return metadata;

}

void store_file(meta metadata, void *buffer, char *sourceFile, unsigned int size){
	file newFile;
	int emptyBlockIndex, i, k;
	strcpy(newFile.file_name, sourceFile);
	newFile.file_length = getFileSize(sourceFile);
	newFile.no_of_blocks = ceil((double(newFile.file_length) / BLOCKSIZE));

	for (i = 1; i < BLOCKNUM; i++){
		if (metadata.blocks[i] == true){
			emptyBlockIndex = i;
			newFile.stack_length = i;
			break;
		}
	}
	metadata.file_pointers[metadata.no_of_files] = newFile;
	metadata.no_of_files++;
	int start = i;

	k = newFile.no_of_blocks;
	while (k > 0){
		metadata.blocks[start++] = false;
		k--;
	}
	FILE *diskFile = fopen(DISKFILENAME, "rb+");
	fseek(diskFile, BLOCKSIZE*i, SEEK_SET);
	fwrite(buffer, size, 1, diskFile);

	fseek(diskFile, 0, SEEK_SET);
	memcpy(buffer, &metadata, sizeof(metadata));
	fwrite(buffer, sizeof(meta), 1, diskFile);
	fclose(diskFile);
}

void set_block(void *buffer, char *fileName){
	meta metadata = get_meta(buffer);
	FILE *sourceFile = fopen(fileName, "rb");
	sourceFile = fopen(fileName, "rb");
	if (getFileSize(fileName) < BLOCKSIZE)
		fread(buffer, getFileSize(fileName), 1, sourceFile);
	else
		fread(buffer, BLOCKSIZE, 1, sourceFile);
	fclose(sourceFile);
	store_file(metadata, buffer, fileName, getFileSize(fileName));

}




void get_block(char *source, char *destination, void *buffer){
	int i=0;
	meta metadata = get_meta(buffer);
	FILE *diskFile = fopen("db.hdd", "rb");
	fread(&metadata, sizeof(meta), 1, diskFile);
	int currNumOfBlocks = 0;
	for (i = 0; i < metadata.no_of_files; i++){
		if (strcmp(metadata.file_pointers[i].file_name, source) == 0) break;
	}
	int startBlock = metadata.file_pointers[i].stack_length;
	fseek(diskFile, BLOCKSIZE * startBlock, SEEK_SET);
	long int fileSize = metadata.file_pointers[i].file_length;
	fread(buffer, BLOCKSIZE, 1, diskFile);
	FILE *destFile = fopen(destination, "wb+");
	fwrite(buffer, BLOCKSIZE, 1, destFile);
	fclose(destFile);
	fclose(diskFile);
	
}

void delete_file(char * source, void *buffer){
	int i = 0;
	meta metadata = get_meta(buffer);
	FILE *diskFile = fopen("db.hdd", "rb+");
	fread(&metadata, sizeof(meta), 1, diskFile);
	//int currNumOfBlocks = 0;
	for (i = 0; i < metadata.no_of_files; i++){
		if (strcmp(metadata.file_pointers[i].file_name, source) == 0) break;
	}
	int startBlock = metadata.file_pointers[i].stack_length;
	int no_of_blocks = metadata.file_pointers[i].no_of_blocks;
	metadata.file_pointers[i] = metadata.file_pointers[metadata.no_of_files-1];
	metadata.no_of_files--;
	while (no_of_blocks > 0){
		metadata.blocks[startBlock++] = true;
		no_of_blocks--;
	}
	fseek(diskFile, 0, SEEK_SET);
	memcpy(buffer, &metadata, sizeof(metadata));
	fwrite(buffer, sizeof(meta), 1, diskFile);
	fclose(diskFile);

}

void printFiles(void *buffer){
	int i;
	meta metadata = get_meta(buffer);
	FILE *diskFile = fopen("db.hdd", "r+");
	FILE *sourceFile;
	for (i = 0; i < metadata.no_of_files; i++){
			printf("%s ", metadata.file_pointers[i].file_name);
		}
	printf("\n");
	}


void test_fileSystem(){
	void *buffer = malloc(BUFFSIZE*sizeof(char));
	format(buffer);
	int i;
	char *command = (char*)calloc(20, sizeof(char));
	char temp[10], source[20], destination[20], ch;
	FILE *filePointer;
	printf("ls\n");
	printf("SET <filename>\n");
	printf("GET <destination> <source>\n");
	do{
		printf("> ");
		i = 0;

		do{
			scanf("%c", &ch);
			if (ch != '\n') command[i++] = ch;
		} while (ch != '\n');

		command[i] = '\0';
	
		if (getCommand(command) == SET){
			sscanf(command, "%s %s %s", temp, source, temp);
			set_block(buffer, source);
		}
		if (getCommand(command) == GET){
			sscanf(command, "%s %s %s", temp, destination, source);
			get_block(source, destination, buffer);

		}

		if (getCommand(command) == DELETE){
			sscanf(command, "%s %s", temp, source);
			delete_file(source, buffer);
		}

		if (getCommand(command) == LS){
			printFiles(buffer);
		}

	} while (true);
	
}