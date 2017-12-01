#ifndef MATRICES_H
#define MATRICES_H

#include <stdio.h>   // printf
#include <stdlib.h>  // malloc and free
#include <complex.h> // complex datatype
#include <time.h>    // To generate a random seed

typedef complex mtype;

typedef struct matrix_t {
    int rows;
    int cols;
    mtype *vals;
} matrix;

matrix* matrixInit(int rows, int cols, mtype *vals);
//matrix* matrixEmpty(matrix *dst, int rows, int cols);
void matrixAdd(matrix *dst, matrix *src1, matrix *src2);
void matrixMul(matrix *dst, matrix *src1, matrix *src2);
void matrixScalarMul(matrix *dst, matrix *src, mtype scalar);
void matrixTrans(matrix *dst, matrix *src);
void matrixSub(matrix *dst, matrix *src, int row, int col);
void matrixIdentity(matrix *dst, int rows);
void matrixComatrix(matrix *dst, matrix *src);
void matrixInverse(matrix *dst, matrix *src);
mtype matrixTrace(matrix *src);
mtype matrixDet(matrix *src);
mtype matrixCofactor(matrix *src, int row, int col);
mtype* matrixEigenvalues(matrix *src);
mtype vectorNorm(matrix *dst, matrix *src);
void matrixPowerIteration(matrix *dst, matrix *src);
complex power(complex base, int exp);
complex sqrtIt(double x);
int isEqual(double x, double y);
void printc(complex c);
void matrixPrint(matrix *src);
void matrixDestroy(matrix *src);
void allMatrixTests(void);
void speedInversionTest(int numMatrices);

#endif

