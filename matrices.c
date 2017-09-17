#include <stdio.h>
#include <stdlib.h>

typedef struct matrix_t {
    int rows;
    int cols;
    int *vals;
} matrix;

matrix* matrixInit(int rows, int cols, int *vals);
matrix* matrixEmpty(int rows, int cols);
matrix* matrixAdd(matrix *m1, matrix *m2);
matrix* matrixMul(matrix *m1, matrix *m2);
matrix* matrixScalarMul(matrix *m, int scalar);
matrix* matrixTrans(matrix *m);
matrix* matrixSub(matrix *m, int row, int col);
int matrixTrace(matrix *m);
int matrixDet(matrix *m);
int power(int base, int exp);
void printMatrix(matrix *m);

int main(void)
{
    int vals1[] = {4, 3, 1, 2};
    int vals2[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int vals3[] = {1, 2, 3, 4, 5, 24, 7, 8, 9, 10, 11, 12, 13, 14, 15, 17};

    matrix *m1 = matrixInit(2, 2, vals1);
    //printMatrix(m1);
    matrix *m2 = matrixInit(3, 3, vals2);
    //printMatrix(m2);
    matrix *m3 = matrixInit(4, 4, vals3);

    printMatrix(matrixSub(m3, 1, 1));
    //printf("Determinant of m1: %d\n", matrixDet(m1));
    //printf("Determinant of m2: %d\n", matrixDet(m2));
    //printf("Determinant of m3: %d\n", matrixDet(m3));

    free(m1);
    free(m2);
    free(m3);

    return 0;
}


/* matrixInit
 * Inputs: The number of rows and columns in a matrix, as well as its
 *  values
 * Outputs: A pointer to a matrix containing that information
 */
matrix* matrixInit(int rows, int cols, int *vals)
{
    matrix *m;
    m = malloc(1);
    (*m).rows = rows;
    (*m).cols = cols;
    (*m).vals = vals;
    return m;
}


/* matrixEmpty
 * Inputs: The number of rows and columns in a matrix
 * Outputs: A pointer to a matrix with enough space for rows*cols values
 *  (uninitialized!)
 */
matrix* matrixEmpty(int rows, int cols)
{
    matrix *m;
    int *vals;
    m = malloc(1);
    vals = malloc(rows*cols);
    (*m).rows = rows;
    (*m).cols = cols;
    (*m).vals = vals;
    return m;
}


/* matrixAdd
 * Inputs: Pointers to two matrices to be added together
 * Outputs: The sum (if defined). Otherwise a null pointer.
 */
matrix* matrixAdd(matrix *m1, matrix *m2)
{
    int rows1 = m1->rows;
    int cols1 = m1->cols;
    int rows2 = m2->rows;
    int cols2 = m2->cols;
    int size  = rows1*cols1;
    int n;

    matrix* ret = matrixInit(rows1, cols1, m1->vals);

    if (rows1 != rows2 || cols1 != cols2) {
        free(ret);
        return NULL;
    }

    for (n = 0; n < size; n++) {
        ret->vals[n] = m1->vals[n] + m2->vals[n];
    }

    return ret;
}


/* matrixMul
 * Inputs: Pointers to two matrices to be multiplied together
 * Outputs: The product (if defined). Otherwise a null pointer.
 */
matrix* matrixMul(matrix *m1, matrix *m2)
{
    int rows1 = m1->rows;
    int cols1 = m1->cols;
    int rows2 = m2->rows;
    int cols2 = m2->cols;

    int row; // Iterator for rows in the first matrix
    int col; // Iterater for columns in the second
    int r;   // Iterator to pull elements from first/second in parallel
    int p;   // Temporary storage for the element in the product matrix

    matrix* product = matrixEmpty(rows1, cols2);
    product->vals[rows1*cols2] = 0;

    // Matrix multiplication is only defined when the number of columns
    // in the first matrix is the same as the number of rows in the
    // second.
    if (cols1 != rows2) {
        free(product);
        return NULL;
    }

    // We look at each individual row in the first matrix
    for (row = 0; row < rows1; row++) {
        // and each individual column in the second
        for (col = 0; col < cols2; col++) {
            p = 0;
            // Now get the sum of products for corresponding elements
            for (r = 0; r < cols1; r++) {
                p += (m1->vals[row*cols1+r] * m2->vals[r*cols2+col]);
            }
            // and write that into our "product" matrix to be returned.
            product->vals[row*cols2+col] = p;
        }
    }

    return product;
}

/* matrixScalarMul
 * Inputs: A pointer to a matrix and a scalar to multiply it
 * Outputs: The scalar product
 */
matrix* matrixScalarMul(matrix *m, int scalar)
{
    int rows = m->rows;
    int cols = m->cols;
    matrix *scaled = matrixEmpty(rows, cols);
    int n;

    for (n = 0; n < rows*cols; n++) {
        scaled->vals[n] = scalar*(m->vals[n]);
    }

    return scaled;
} 


/* matrixTrans
 * Inputs: A pointer to a matrix
 * Outputs: A pointer to a matrix that is the transposition of the input
 */
matrix* matrixTrans(matrix *m)
{
    int rows = m->rows;
    int cols = m->cols;
    matrix *trans = matrixEmpty(cols, rows);

    int i, j;

    for (i = 0; i < cols; i++) {
        for (j = 0; j < rows; j++) {
            trans->vals[i*rows+j] = m->vals[j*cols+i];
        }
    }
    return trans;
}

/* matrixSub
 * Inputs: A pointer to a matrix and the row and column to remove
 * Outputs: A pointer to a matrix without that row and column
 */
matrix* matrixSub(matrix *m, int row, int col)
{
    int rows = m->rows;
    int cols = m->cols;
    int i; // The counter for elements being put into the sub-matrix
    int n; // The counter for elements being taken from the main matrix
    matrix *sub = matrixEmpty(rows-1, cols-1);

    if (row > rows || col > cols) {
        return m;
    }

    //printf("\n");
    i = 0;
    for (n = 0; n < rows*cols; n++) {
        if ((n-col) % (cols) == 0) {
            ;
        } else if (row*cols <= n && n < ((row+1)*cols)) {
            ;
        } else {
            printf("i=%d n=%d m->vals[n]=%d\n", i, n, m->vals[n]);
            sub->vals[i] = m->vals[n];
            i++; // Only increment i when we've added to the sub-matrix
        }
    }
    //printf("\n");

    return sub;
}

/* matrixTrace
 * Inputs: A pointer to a square matrix
 * Outputs: The trace (sum of the diagonals) of that matrix. If a non-
 *  square matrix is passed, return the largest possible 32-bit value.
 */
int matrixTrace(matrix *m)
{
    int rows = m->rows;
    int cols = m->cols;
    int trace = 0;
    int n;

    if (rows != cols) {
        return 0xffffffff;
    }

    for (n = 0; n < cols; n++) {
        trace += m->vals[n+n*cols];
        trace += m->vals[n*cols-n];
    }

    return trace;
}

int matrixDet(matrix *m)
{
    int rows = m->rows;
    int cols = m->cols;
    int n;
    int det;

    if (rows != cols) {
        return 0xffffffff;
    } else if (rows == 2) {
        return (m->vals[0]*m->vals[3]) - (m->vals[1]*m->vals[2]);
    } else {
        det = 0;
        for (n = 0; n < cols; n++) {
            det += m->vals[n]*power(-1, n)*matrixDet(matrixSub(m, 0, n));
        }
    }
    return det;
}

int power(int base, int exp) {
    int result = base;
    if (exp == 0) {
        return 1;
    }
    for ( ; exp > 1; exp--) {
        result *= base;
    }
    return result;
}

/* printMatrix
 * Inputs: A pointer to the matrix to be printed
 * Outputs: None, the matrix is printed with rough formatting
 */
void printMatrix(matrix *m)
{
    int rows = m->rows;
    int cols = m->cols;
    printf("rows=%d cols=%d\n", rows, cols);
    /*
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%d\t", m->vals[cols*i+j]);
        }
        printf("\n");
    }
    printf("\n");
    */
    for (rows = 0; rows < 9; rows++) {
        printf("%d\n ", m->vals[rows]);
    }
    return;
}

