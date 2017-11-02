#include "matrices.h"


/* Temporary fix for initializing a matrix without passing values */
#define EMPTY &EMPTY_VALS
static double EMPTY_VALS = {0};

/* matrixInit
 * Inputs: The number of rows and columns in a matrix, as well as its
 *  values
 * Outputs: A pointer to a matrix containing that information
 */
matrix* matrixInit(int rows, int cols, double *vals)
{
    int i;
    matrix *dst = malloc(sizeof(matrix));
    (*dst).rows = rows;
    (*dst).cols = cols;
    (*dst).vals = (double *)malloc(rows*cols*sizeof(double));
    if (sizeof(vals) == sizeof(double) && vals[0] == 0) {
        for (i = 0; i < rows*cols; i++) {
            (*dst).vals[i] = 0;
        }
    } else {
        for (i = 0; i < rows*cols; i++) {
            (*dst).vals[i] = vals[i];
        }
    }
    return dst;
}

void matrixRealloc(matrix *src, int rows, int cols)
{
    //int i;
    (*src).rows = rows;
    (*src).cols = cols;
    free(src->vals);
    (*src).vals = (double *)malloc(rows*cols*sizeof(double));
    //for (i = 0; i < rows*cols; i++) {
    //    (*src).vals[i] = 0;
    //}
    return;
}



/* matrixEmpty
 * Inputs: The number of rows and columns in a matrix
 * Outputs: A pointer to a matrix with enough space for rows*cols values
 *  (uninitialized!)
 */
void matrixEmpty(matrix *dst, int rows, int cols)
{
    double *vals;
    vals = malloc(sizeof(double)*rows*cols);
    (*dst).rows = rows;
    (*dst).cols = cols;
    (*dst).vals = vals;
    return;
}


/* matrixAdd
 * Inputs: Pointers to two matrices to be added together
 * Outputs: The sum (if defined). Otherwise a null pointer.
 */
void matrixAdd(matrix *dst, matrix *src1, matrix *src2)
{
    int rows1 = src1->rows;
    int cols1 = src1->cols;
    int rows2 = src2->rows;
    int cols2 = src2->cols;
    int size  = rows1*cols1;
    int n;

    if (rows1 != rows2 || cols1 != cols2) {
        return;
    }

    for (n = 0; n < size; n++) {
        dst->vals[n] = src1->vals[n] + src2->vals[n];
    }

    return;
}


/* matrixMul
 * Inputs: Pointers to two matrices to be multiplied together
 * Outputs: The product (if defined). Otherwise a null pointer.
 */
void matrixMul(matrix *dst, matrix *src1, matrix *src2)
{
    int rows1 = src1->rows;
    int cols1 = src1->cols;
    int rows2 = src2->rows;
    int cols2 = src2->cols;

    int row;  // Iterator for rows in the first matrix
    int col;  // Iterater for columns in the second
    int r;    // Iterator to pull elements from first/second in parallel
    double p; // Temporary storage for the element in the product matrix

    /* Matrix multiplication is only defined when the number of columns
     * in the first matrix is the same as the number of rows in the
     * second.
     * */
    if (cols1 != rows2) {
        return;
    }

    // We look at each individual row in the first matrix
    for (row = 0; row < rows1; row++) {
        // and each individual column in the second
        for (col = 0; col < cols2; col++) {
            p = 0;
            // Now get the sum of products for corresponding elements
            for (r = 0; r < cols1; r++) {
                p += (src1->vals[row*cols1+r] * src2->vals[r*cols2+col]);
            }
            // and write that into our "product" matrix to be returned.
            dst->vals[row*cols2+col] = p;
        }
    }
    return;
}

/* matrixScalarMul
 * Inputs: A pointer to a matrix and a scalar to multiply it
 * Outputs: The scalar product
 */
void matrixScalarMul(matrix *dst, matrix *src, double scalar)
{
    int rows = src->rows;
    int cols = src->cols;
    int n;

    for (n = 0; n < rows*cols; n++) {
        dst->vals[n] = scalar*(src->vals[n]);
    }
    return;
} 


/* matrixTrans
 * Inputs: A pointer to a matrix
 * Outputs: A pointer to a matrix that is the transposition of the input
 */
void matrixTrans(matrix *dst, matrix *src)
{
    int rows = src->rows;
    int cols = src->cols;
    int i, j;

    for (i = 0; i < cols; i++) {
        for (j = 0; j < rows; j++) {
            dst->vals[i*rows+j] = src->vals[j*cols+i];
        }
    }
    return;
}

/* matrixSub
 * Inputs: A pointer to a matrix and the row and column to remove
 * Outputs: A pointer to a matrix without that row and column
 */
void matrixSub(matrix *dst, matrix *src, int row, int col)
{
    int rows = src->rows;
    int cols = src->cols;
    int i; // The counter for elements being put into the sub-matrix
    int n; // The counter for elements being taken from the main matrix

    if (row > rows || col > cols) {
        return;
    }

    i = 0;
    for (n = 0; n < rows*cols; n++) {
        if ((n-col) % (cols) == 0) {
            ;
        } else if (row*cols <= n && n < ((row+1)*cols)) {
            ;
        } else {
            dst->vals[i] = src->vals[n];
            i++; // Only increment i when we've added to the sub-matrix
        }
    }
}

/* matrixIdentity
 * Inputs: The number of rows (columns) that the  matrix is to contain
 * Outputs: An identity matrix of the given size
 */
void matrixIdentity(matrix *dst, int rows)
{
    int i;
    for (i = 0; i < rows*rows; i++) {
        dst->vals[i] = 0;
    }
    for (i = 0; i < rows; i++) {
        dst->vals[i*rows+i] = 1;
    }
}


/* matrixComatrix
 * Inputs: A pointer to a matrix
 * Outputs: A matrix containing the cofactors of the input
 */
void matrixComatrix(matrix *dst, matrix *src)
{
    int i, j;
    double co;
    for (i = 0; i < src->cols; i++) {
        for (j = 0; j < src->rows; j++) {
            co = matrixCofactor(src, i, j);
            dst->vals[i*(src->cols)+j] = co;
        }
    }
}

void matrixInverse(matrix *dst, matrix *src)
{
    double det = matrixDet(src);
    matrix* trans = matrixInit(src->cols, dst->cols, EMPTY);
    matrixComatrix(dst, src);
    if (det == 0) {
        return;
    }

    matrixTrans(trans, dst);
    matrixScalarMul(dst, trans, 1/det);
    matrixDestroy(trans);
}

/* matrixTrace
 * Inputs: A pointer to a square matrix
 * Outputs: The trace (sum of the diagonals) of that matrix. If a non-
 *  square matrix is passed, return the largest possible 32-bit value.
 */
double matrixTrace(matrix *src)
{
    int rows = src->rows;
    int cols = src->cols;
    double trace = 0;
    int n;

    if (rows != cols) {
        return 0xdeadbeef;
    }

    for (n = 0; n < cols; n++) {
        trace += src->vals[n+n*cols];
        trace += src->vals[n*cols-n];
    }

    return trace;
}

double matrixDet(matrix *src)
{
    int rows = src->rows;
    int cols = src->cols;
    int n;
    double det;
    matrix *tempSub;

    if (rows != cols) {
        return 0xdeafbeef;
    } else if (rows == 2) {
        /* The definition of the determinant for a 2x2 matrix */
        return (src->vals[0]*src->vals[3]) - (src->vals[1]*src->vals[2]);
    } else {
        det = 0;
        for (n = 0; n < cols; n++) {
            /* Reduce the rank */
            tempSub = matrixInit(rows-1, cols-1, EMPTY);
            matrixSub(tempSub, src, 0, n);
            det += src->vals[n]*power(-1, n)*matrixDet(tempSub);
            //det += m->vals[n]*power(-1, n)*matrixDet(matrixSub(m, 0, n));
            matrixDestroy(tempSub);
        }
    }
    return det;
}

/* matrixCofactor
 * Inputs: A pointer to a matrix, and the row and column containing the
 *  element whose cofactor we want
 * Output: The cofactor of the element specified
 */
double matrixCofactor(matrix *src, int row, int col)
{
    matrix* sub = matrixInit(src->rows-1, src->cols-1, EMPTY);
    matrixSub(sub, src, row, col);
    double co = matrixDet(sub);
    matrixDestroy(sub);
    co *= power(-1, row+col);
    return co;
}

/* power
 * Inputs: A base (positive or negative) and exponent (non-negative)
 * Outputs: base^power
 */
int power(int base, int exp)
{
    int result = base;
    if (exp == 0) {
        return 1;
    }
    for ( ; exp > 1; exp--) {
        result *= base;
    }
    return result;
}

/* matrixPrint
 * Inputs: A pointer to the matrix to be printed
 * Outputs: None, the matrix is printed with rough formatting
 */
void matrixPrint(matrix *src)
{
    int rows = src->rows;
    int cols = src->cols;
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%.3lg\t", src->vals[cols*i+j]);
        }
        printf("\n");
    }
    printf("\n");
    return;
}

void matrixDestroy(matrix *src)
{
    free(src->vals);
    src->vals = NULL;
    free(src);
    src = NULL;
    return;
}

void allMatrixTests(void)
{
    double vals[] = {1, 4, 7, 3, 0, 5, -1, 9, 11};
    double scalar = 2.5;
    matrix *m = matrixInit(3, 3, vals);
    matrix *n = matrixInit(3, 3, EMPTY);

    printf("The matrix we're working with is:\n");
    matrixPrint(m);
    printf("\nThe matrix added to itself is:\n");
    matrixAdd(n, m, m);
    matrixPrint(n);
    printf("\nThe matrix multiplied by itself is:\n");
    matrixMul(n, m, m);
    matrixPrint(n);
    printf("\nThe matrix multiplied by %lf is:\n", scalar);
    matrixScalarMul(n, m, scalar);
    matrixPrint(n);
    printf("\nThe transposition of the matrix is:\n");
    matrixTrans(n, m);
    matrixPrint(n);
    printf("\nThe sub-matrix with the middle row/column removed is:\n");
    matrixSub(n, m, 1, 1);
    matrixPrint(n);
    printf("\nThe corresponding identity matrix is:\n");
    matrixIdentity(n, 3);
    matrixPrint(n);
    printf("\nThe comatrix is:\n");
    matrixComatrix(n, m);
    matrixPrint(n);
    printf("\nThe inverse matrix is:\n");
    matrixInverse(n, m);
    matrixPrint(n);
    printf("\nThe trace is: %.3lf\n", matrixTrace(m));
    printf("\nThe determinant is: %.3lf\n", matrixDet(m));

    matrixDestroy(m);
    matrixDestroy(n);
    return;
}

void speedInversionTest(int numMatrices)
{
    srand(time(NULL));
    int i;
    matrix *m = matrixInit(3, 3, EMPTY);
    matrix *n = matrixInit(3, 3, EMPTY);
    for (; numMatrices > 0; numMatrices--) {
        // Generate 9 random numbers between 0 and 63
        for (i = 0; i < 9; i++) {
            m->vals[i] = (rand() % 64);
        }
        matrixInverse(n, m);
    }
    matrixDestroy(m);
    matrixDestroy(n);
    return;
}

