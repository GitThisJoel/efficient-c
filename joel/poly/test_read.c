#include <stdio.h>
#include <string.h>
void read_from_string(const char* input) {
	for (int i = 0; i < strlen(input); i++) {
		printf("%c\n", input[i]);
	}
}

int main(void) {
	read_from_string("x^123 + x^10");
}
