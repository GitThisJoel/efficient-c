#include <stdio.h>
#include <math.h>

int main()
{
  double  pi = 4 * atan(1.0);

  printf("%10.5lf", pi);
  printf("%10.5lf\n", pi);
  printf("%10.5lf", -pi);
  printf("% 10.5lf\n", pi);
}
