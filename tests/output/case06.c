#include <stdio.h>
#include <string.h>
/// [multiarray] created on 2025/5/10
int matrix[2][2];
int i, j;
int main() {
  for (i = 1; i <= 2; i++) {
    for (j = 1; j <= 2; j++) {
      matrix[i - 1][j - 1] = i * j;
    }
  }
  printf("%d\n", matrix[2 - 1][1 - 1]);
  return 0;
}
