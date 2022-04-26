#include <stdio.h>
int main(void)
{
  signed int  a = 1;
  signed int* p = &a;
  signed short* q = (signed short*)p;

  *q = -1;
  
  printf("a = %d\n", a);
  
  return 0;
}
