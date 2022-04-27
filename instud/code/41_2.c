#include <stdio.h>

void print_list(int* xs, int n, int m) {
	int i, j;
	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++)
			printf("%d ", xs[i][j]);
		printf("\n");
	}
}

int main() {
	print_list( (int[][]) {{1}, {2}, {3}, {4}, {5}}, 5, 1);	
	return 0;
}

