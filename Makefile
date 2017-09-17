test: clean matrices
	./matrices

matrices: matrices.c
	gcc -Wall -g matrices.c -o matrices

clean:
	rm -f matrices
