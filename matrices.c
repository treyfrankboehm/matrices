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
void printMatrix(matrix *m1);

int main(void)
{
    matrix *m1;
    matrix *m2;
    matrix *m3;

    int vals1[] = {1, 2, 3, 4, 5, 6};
    int vals2[] = {7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18};

    m1 = matrixInit(3, 2, vals1);
    m2 = matrixInit(4, 3, vals2);

    printMatrix(m1);
    printMatrix(m2);

    m3 = matrixMul(m2, m1);

    printMatrix(m3);

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


/* printMatrix
 * Inputs: A pointer to the matrix to be printed
 * Outputs: The matrix roughly formatted
 */
void printMatrix(matrix *m1)
{
    int rows = m1->rows;
    int cols = m1->cols;
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%d\t", m1->vals[cols*i+j]);
        }
        printf("\n");
    }
    printf("\n");
    return;
}

