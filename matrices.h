#ifndef MATRICES_H
#define MATRICES_H

#include <stdio.h>
#include <stdlib.h>
// I use time to generate a random seed for the speedInversionTest
#include <time.h>

typedef struct matrix_t {
    int rows;
    int cols;
    double *vals;
} matrix;

matrix* matrixInit(int rows, int cols, double *vals);
//matrix* matrixEmpty(matrix *dst, int rows, int cols);
void matrixAdd(matrix *dst, matrix *src1, matrix *src2);
void matrixMul(matrix *dst, matrix *src1, matrix *src2);
void matrixScalarMul(matrix *dst, matrix *src, double scalar);
void matrixTrans(matrix *dst, matrix *src);
void matrixSub(matrix *dst, matrix *src, int row, int col);
void matrixIdentity(matrix *dst, int rows);
void matrixComatrix(matrix *dst, matrix *src);
void matrixInverse(matrix *dst, matrix *src);
double matrixTrace(matrix *src);
double matrixDet(matrix *dst, matrix *src);
double matrixCofactor(matrix *dst, matrix *src, int row, int col);
int power(int base, int exp);
void matrixPrint(matrix *src);
void matrixDestroy(matrix *src);
void allMatrixTests(void);
void speedInversionTest(int numMatrices);

#endif

