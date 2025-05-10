#include <math.h>
#include <stdio.h>
#include <string.h>

#define sqrt(x) (sqrt((float)x))
/// [floatmath] created on 2025/5/10
float x;
int main() {
  x = 3.14;
  printf("%.2f\n", x * 2);
  printf("%.2f\n", sqrt(x));
  return 0;
}
