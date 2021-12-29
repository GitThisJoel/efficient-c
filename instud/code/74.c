#define N (1000)
double a[N][N], b[N][N], c[N][N];
void main(void)
{
  size_t i, j, k;
  for (i = 0; i < N; i += 1) {
    for (j = 0; j < N; j += 1) {
      a[i][j] = 0;
      for (k = 0; k < N; k += 1)
        a[i][j] += b[i][k] * c[k][j];
    }
  }
}
