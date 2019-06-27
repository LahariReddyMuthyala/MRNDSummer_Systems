#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define DISKSIZE 100*1024*1024
#define BUFFSIZE 16*1024
#define BLOCKSIZE 16*1024
#define NUMBEROFBLOCKS 6400
#define DISKNAME "db.hdd"


struct file{
	char file_name[20];
	unsigned int stack_length, no_of_blocks, file_length;
};

struct meta{
	int magic;
	struct file file_pointers[150];
	int no_of_files;
	int no_of_free_blocks;
	bool blocks[NUMBEROFBLOCKS];
};

typedef struct meta meta;
typedef struct file file;

void writeblock(void *buffer, int blockNum){
	FILE *diskFile = fopen(DISKNAME, "rb+");
	if (diskFile == NULL){
		printf("Error opening file");
	}
	fseek(diskFile, blockNum*BLOCKSIZE, SEEK_SET);
	fwrite(buffer, BLOCKSIZE, 1, diskFile);
	fclose(diskFile);
}

void readblock(void *buffer, int blockNum){
	FILE *diskFile = fopen(DISKNAME, "rb");
	if (diskFile == NULL){
		printf("Error opening file");
	}
	fseek(diskFile, BLOCKSIZE*blockNum, SEEK_SET);
	fread(buffer, BLOCKSIZE, 1, diskFile);
	fclose(diskFile);
}

meta get_meta_diskIO(void *buffer){
	int reform;
	meta metadata;
	readblock(buffer, 0);
	memcpy(&metadata, buffer, sizeof(meta));
	/*if (metadata.magic != 0x444E524D){
	printf("Disk is corrupted. Do you want to reformat? (0/1)");
	scanf("%d", &reform);
	if (reform){
	format(buffer);
	return get_meta(buffer);
	}
	}*/
	return metadata;
}

void format_diskIO(void *buffer){
	meta metadata;
	metadata.magic = 0x444E524D;
	metadata.no_of_files = 0;
	int i;
	metadata.blocks[0] = false;
	metadata.no_of_free_blocks = NUMBEROFBLOCKS;
	for (i = 1; i < NUMBEROFBLOCKS; i++){
		metadata.blocks[i] = true;
	}
	memcpy(buffer, &metadata, sizeof(metadata));
	writeblock(buffer, 0);
}


void print_list(void *buffer){
	int i;
	meta metadata = get_meta_diskIO(buffer);
	for (i = 0; i < metadata.no_of_files; i++){
		printf("%s ", metadata.file_pointers[i].file_name);
	}
	printf("\n");
}

long int getFileSize_diskIO(char *fileName){
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

void copy_from_FS(char *source, char* destination, void* buffer){
	file newFile; 
	int i;
	meta metadata = get_meta_diskIO(buffer);
	strcpy(newFile.file_name, destination);
	newFile.file_length = getFileSize_diskIO(source);
	double x = double((newFile.file_length) / BLOCKSIZE);
	newFile.no_of_blocks = ceil((double(newFile.file_length) / (BLOCKSIZE)));

	FILE *sourceFile = fopen(source, "rb");
	if (newFile.no_of_blocks > 1){
		int *blockIndices = (int*)malloc(sizeof(int)*(newFile.no_of_blocks + 1));
		int k = 0;
		for (i = 0; i < NUMBEROFBLOCKS; i++){
			if (k == newFile.no_of_blocks + 1){
				break;
			}
			if (metadata.blocks[i] == true){
				blockIndices[k++] = i;
			}
		}
		newFile.stack_length = blockIndices[0];
		memcpy(buffer, blockIndices+1, newFile.no_of_blocks);
		printf("%s\n", buffer);
		writeblock(buffer, blockIndices[0]);
		metadata.blocks[blockIndices[0]] = false;
		for (i = 1; i < newFile.no_of_blocks+1; i++){
			metadata.blocks[blockIndices[i]] = false;
			fread(buffer, BLOCKSIZE, 1, sourceFile);
			writeblock(buffer, blockIndices[i]);
		}

	}

	else{
		for (i = 1; i < NUMBEROFBLOCKS; i++){
			if (metadata.blocks[i] == true){
				newFile.stack_length = i;
				break;
			}
		}
		fread(buffer, getFileSize_diskIO(source), 1, sourceFile);
		writeblock(buffer,i);
		metadata.blocks[i] = false;
	}
	metadata.file_pointers[metadata.no_of_files] = newFile;
	metadata.no_of_files++;
	metadata.no_of_free_blocks -= newFile.no_of_blocks;
	memcpy(buffer, &metadata, sizeof(metadata));
	printf("%s\n", buffer);
	writeblock(buffer, 0);
}

void copy_to_FS(char *source, char *destination, void* buffer){
	meta metadata = get_meta_diskIO(buffer);
	FILE *destFile = fopen(destination, "wb+");
	int i, j;
	for (i = 0; i < metadata.no_of_files; i++){
		if (strcmp(metadata.file_pointers[i].file_name, source) == 0){
			break;
		}
	}
	if (metadata.file_pointers[i].no_of_blocks == 1){
		readblock(buffer, metadata.file_pointers[i].stack_length);
		fwrite(buffer, getFileSize_diskIO(source), 1, destFile);
	}
	if (metadata.file_pointers[i].no_of_blocks > 1){
		int no_of_blocks = (int)metadata.file_pointers[i].no_of_blocks;
		int *blockIndices = (int*)calloc(sizeof(int),no_of_blocks);
		readblock(buffer, metadata.file_pointers[i].stack_length);
		memcpy(blockIndices, buffer, no_of_blocks*sizeof(int));
		for (j = 0; j < no_of_blocks; j++){
			readblock(buffer, blockIndices[j]);
			fwrite(buffer, BLOCKSIZE, 1, destFile);
		}
		
	}
	else{
		printf("File not found\n");
		return;
	}
	fclose(destFile);
}

void delete_file(char *fileName, void* buffer){
	meta metadata = get_meta_diskIO(buffer);
	int i, j;
	for (i = 0; i < metadata.no_of_files; i++){
		if (strcmp(metadata.file_pointers[i].file_name, fileName) == 0){
			break;
		}
	}


}

void getCommand(char *input, void* buffer){
	char command[20], temp[20], source[30], destination[30];
	sscanf(input, "%s %s", command, temp);
	int i;
	for (i = 0; command[i] != '\0'; i++){
		if ('a' <= command[i] && command[i] <= 'z') command[i] -= 32;
	}

	if (strcmp(command, "COPYTOFS") == 0){
		sscanf(input, "%s %s %s",command, source, destination);
		copy_to_FS(source, destination, buffer);
	}

	else if (strcmp(command, "COPYFROMFS") == 0){
		sscanf(input, "%s %s %s",command, source, destination);
		copy_from_FS(source, destination, buffer);

	}
	if (strcmp(command, "LS") == 0)
		print_list(buffer);
	if (strcmp(command, "DELETE") == 0){
		//delete_file(temp);
	}
	if (strcmp(command, "DEBUG") == 0)
		//debug(buffer);
	if (strcmp(command, "MOUNT") == 0){
		strcpy(DISKNAME, temp);
	}
	//if (strcmp(command, "UNMOUNT") == 0) return UNMOUNT;
}

void test_diskIO(){
	void *buffer = malloc(sizeof(char)*BUFFSIZE);
	char *command = (char*)calloc(30, sizeof(char));
	char ch;
	int i = 0;
	format_diskIO(buffer);
	do{
		printf("> ");
		i = 0;
		do{
			scanf("%c", &ch);
			if (ch != '\n') command[i++] = ch;
		} while (ch != '\n');

		command[i] = '\0';
		getCommand(command, buffer);

	} while (1);


}