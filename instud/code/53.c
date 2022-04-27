#include <stdio.h>
#include <alloca.h>

void f(int n)
{
  int i;
  int* p;
  for (i = 0; i < n; i += 1) {
    int a[i];
    p = alloca(i * sizeof(int));
    /* use a and p here... */

    a[0] = 1;
    p[0] = 1;
  }
  p[0];
  //a[0];
}

int main() {
	f(1);
	return 0;
}
