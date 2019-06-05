#include <stdio.h>
#include <stdlib.h>

int validBounds(int currRow, int currCol, int rows, int cols){
	if (currRow > -1 && currRow<rows && currCol>-1 && currCol < cols)
		return 1;
	return 0;
}

int check(int matrix[][5], int dir, int rows, int cols, char str[], int index, int currRow, int currCol, int len, int startRow, int startCol){
	if (index == len){
		printf("Start = %d %d", startRow, startCol);
		printf(" End = %d %d\n", currRow, currCol);
		return 1;
	}
	if (dir == -1){
		if (validBounds(currRow, currCol - 1, rows, cols) && matrix[currRow][currCol - 1] == str[index] && currCol >= len - 1){
			check(matrix, 0, rows, cols, str, index + 1, currRow, currCol - 1, len, startRow, startCol);
		}
		if (validBounds(currRow - 1, currCol - 1, rows, cols) && matrix[currRow - 1][currCol - 1] == str[index] && currCol >= len - 1 && currRow >= len - 1){
			check(matrix, 1, rows, cols, str, index + 1, currRow-1, currCol - 1, len, startRow, startCol);
		}
		if (validBounds(currRow - 1, currCol, rows, cols) && matrix[currRow - 1][currCol] == str[index] && currRow >= len - 1){
			check(matrix, 2, rows, cols, str, index + 1, currRow - 1, currCol, len, startRow, startCol);
		}
		 if (validBounds(currRow - 1, currCol +1, rows, cols) && matrix[currRow - 1][currCol + 1] == str[index]){
			check(matrix, 3, rows, cols, str, index + 1, currRow - 1, currCol + 1, len, startRow, startCol);
		}
		 if (validBounds(currRow , currCol + 1, rows, cols) && matrix[currRow][currCol + 1] == str[index]){
			check(matrix, 4, rows, cols, str, index + 1, currRow, currCol + 1, len, startRow, startCol);
		}
		 if (validBounds(currRow + 1, currCol + 1, rows, cols) && matrix[currRow + 1][currCol + 1] == str[index]){
			check(matrix, 5, rows, cols, str, index + 1, currRow + 1, currCol + 1, len, startRow, startCol);
		}
		 if (validBounds(currRow + 1, currCol , rows, cols) && matrix[currRow + 1][currCol] == str[index]){
			check(matrix, 6, rows, cols, str, index + 1, currRow + 1, currCol, len, startRow, startCol);
		}
		 if (validBounds(currRow + 1, currCol - 1, rows, cols) && matrix[currRow + 1][currCol - 1] == str[index]){
			check(matrix, 7, rows, cols, str, index + 1, currRow + 1, currCol - 1, len, startRow, startCol);
		}
		else
			return 0;

	}
	else if (dir == 0){
		if (validBounds(currRow, currCol - 1, rows, cols) && matrix[currRow][currCol - 1] == str[index] && currCol>=len-1){
			check(matrix, 0, rows, cols, str, index + 1, currRow, currCol - 1, len, startRow, startCol);
		}
		else
			return 0;
	}
	else if (dir == 1){
		if (validBounds(currRow - 1, currCol - 1, rows, cols) && matrix[currRow - 1][currCol - 1] == str[index] && currCol >= len - 1 && currRow>=len-1){
			check(matrix, 1, rows, cols, str, index + 1, currRow - 1, currCol - 1, len, startRow, startCol);
		}
		else
			return 0;
	}
	else if (dir == 2){
		if (validBounds(currRow - 1, currCol, rows, cols) && matrix[currRow - 1][currCol] == str[index] && currRow>=len-1){
			check(matrix, 2, rows, cols, str, index + 1, currRow - 1, currCol, len, startRow, startCol);
		}
		else
			return 0;
	}
	else if (dir == 3){
		if (validBounds(currRow - 1, currCol + 1, rows, cols) && matrix[currRow - 1][currCol + 1] == str[index] && cols-currCol<=len-1){
			check(matrix, 3, rows, cols, str, index + 1, currRow - 1, currCol + 1, len, startRow, startCol);
		}
		else
			return 0;
	}
	else if (dir == 4){
		if (validBounds(currRow, currCol + 1, rows, cols) && matrix[currRow][currCol + 1] == str[index] && cols - currCol <= len - 1){
			check(matrix, 4, rows, cols, str, index + 1, currRow, currCol + 1, len, startRow, startCol);
		}
		else
			return 0;
	}
	else if (dir == 5){
		if (validBounds(currRow + 1, currCol + 1, rows, cols) && matrix[currRow + 1][currCol + 1] == str[index] && rows - currRow <= len-1 && cols-currCol<=len-1){
			check(matrix, 5, rows, cols, str, index + 1, currRow + 1, currCol + 1, len, startRow, startCol);
		}
		else
			return 0;
	}
	else if (dir == 6){
		if (validBounds(currRow + 1, currCol, rows, cols) && matrix[currRow + 1][currCol] == str[index] && rows-currRow<=len-1){
			check(matrix, 6, rows, cols, str, index + 1, currRow + 1, currCol, len, startRow, startCol);
		}
		else
			return 0;
	}
	else if (dir == 7){
		if (validBounds(currRow + 1, currCol - 1, rows, cols) && matrix[currRow + 1][currCol - 1] == str[index] && currCol>=len-1){
			check(matrix, 7, rows, cols, str, index + 1, currRow + 1, currCol - 1, len, startRow, startCol);
		}
		else
			return 0;
	}
	
}



void getOccurances(int matrix[][5], int rows, int cols, char str[], int len){
	int i, j;

	for (i = 0; i < rows; i++){
		for (j = 0; j < cols; j++){
			if (matrix[i][j] == str[0]){
				check(matrix, -1, rows, cols, str, 1, i, j, len, i, j);
			}
		}
	}
}

void test_crossword(){
	printf("\nCROSSWORD\n");
	//int matrix[][5] = { { 'a', 'b', 'c', 'd', 'e' }, { 'f', 'g', 'h', 'i', 'k' }, { 'f', 'g', 'h', 'h', 'i' }, { 'j', 'k', 'l', 'i', 'k' } };
	int matrix[][5] = { { 'h', 'i', 'h', 'i', 'h' }, { 'h', 'e', 'i', 'e', 'h' } };
	getOccurances(matrix,2,5,"hi", 2);

}