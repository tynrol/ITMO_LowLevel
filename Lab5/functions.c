#include <stdio.h>

void print_row(int elem) {
	printf("%d ", elem);
}

void print_column(int elem) {
	printf("%d\n", elem);
}

int square(int elem) {
	return elem * elem;
}

int cube(int elem) {
	return elem * elem * elem;
}

int sum(int elem, int a) {
	return elem + a;
}

int min(int elem, int a) {
	return elem > a ? a : elem;
}

int max(int elem, int a) {
	return elem > a ? elem : a;
}

int abs(int elem) {
	return elem > 0 ? elem : -1 * elem;
}

int pow_two(int elem) {
	return elem * 2;
}
