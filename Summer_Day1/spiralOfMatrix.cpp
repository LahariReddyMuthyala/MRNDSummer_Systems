#include <stdio.h>
#include <stdlib.h>

void display(int **matrix, int m, int n){
	int i, j;
	for (i = 0; i<m; i++){
		for (j = 0; j<n; j++){
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
}

int* getSpiral(int **matrix, int row, int col){
	int *spiral = (int*)malloc(sizeof(int)*row*col);
	int index = 0;
	int iter, rowStart = 0, colStart = 0, rowEnd = row, colEnd = col;

	while (rowStart < rowEnd && colStart < colEnd){
		for (iter = colStart; iter < colEnd; iter++){
			spiral[index++] = matrix[rowStart][iter];
		}
		rowStart++;

		
		for (iter = rowStart; iter < rowEnd; iter++){
			spiral[index++] = matrix[i][colEnd-1];
		}
		colEnd--;

		if (rowStart < rowEnd){
			for (iter = colEnd - 1; iter >= colStart; iter--){
				spiral[index++] = matrix[rowEnd-1][iter];
			}
			rowEnd--;
		}

		if (colStart < colEnd){
			for (iter = rowEnd - 1; iter >= rowStart; iter--){
				spiral[index++] = matrix[iter][colStart];
			}
			colStart++;
		}
	}
	return spiral;
}

void spiral(){
	printf("\nFINDING SPIRAL OF MATRIX\n");
	int rows, cols, i, j;
	printf("Enter number of rows: ");
	scanf_s("%d", &rows);
	printf("\nEnter number of cols: ");
	scanf_s("%d", &cols);
	int **matrix = (int**)malloc(sizeof(int*) * rows);
	for (i = 0; i < rows; i++){
		matrix[i] = (int*)malloc(sizeof(int) * cols);
	}
	for (i = 0; i < rows; i++){
		for (j = 0; j < cols; j++){
			scanf_s("%d", &matrix[i][j]);
		}
	}
	display(matrix, rows, cols);
	int *spiral = getSpiral(matrix, rows, cols);
	for (i = 0; i < rows*cols; i++){
		printf("%d ", spiral[i]);
	}

}

