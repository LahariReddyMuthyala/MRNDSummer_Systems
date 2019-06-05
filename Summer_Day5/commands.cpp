#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*void getCommand(char *input){
	char command[20], temp[20], source[30], destination[30];
	sscanf(input, "%s %s", command, temp);

	if (strcmp(command, "COPYTOFS") == 0){
		sscanf(temp, "%s %s", source, destination);
		copy_to_FS(source, destination);
	}

	else if (strcmp(command, "COPYFROMFS") == 0){
		sscanf(temp, "%s %s", source, destination);
		copy_from_FS(source, destination);

	}
	if (strcmp(command, "LS") == 0) return LS;
	if (strcmp(command, "DELETE") == 0) return DELETE;
	if (strcmp(command, "DEBUG") == 0) return DEBUG;
	if (strcmp(command, "MOUNT") == 0) return MOUNT;
	if (strcmp(command, "UNMOUNT") == 0) return UNMOUNT;
	return 0;
}*/