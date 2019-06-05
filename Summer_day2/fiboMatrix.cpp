#include <stdio.h>
#include <stdlib.h>

int** matrixMul(int **mat1,int m1, int n1, int** mat2, int m2, int n2){
	if (n1 != m2) return NULL;
	int **product = (int**)malloc(sizeof(int*)*m1);
	int i, j, k;
	for (i = 0; i < m1; i++){
		product[i] = (int*)malloc(sizeof(int)*n2);
	}
	for (i = 0; i < m1; i++){
		for (j = 0; j < n2; j++){
			for (k = 0; k < m2; k++){
				product[i][j] += mat1[i][k] * mat2[k][j];
			}
		}
	}
	return product;
}


int** matrixExp(int **matrix, int n){
	int **p = matrixExp(matrix, n / 2);
	p = matrixMul(p,2,2,p,2,2);
	if (n % 2 == 0)
		return matrixMul(p,2,2, p,2,2);
	else
		return matrixMul(matrixMul(p,2,2,p,2,2),2,2,matrix,2,2);
}