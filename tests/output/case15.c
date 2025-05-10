#include <stdio.h>
#include <string.h>
#define round(x) ((int)(x + 0.5))
/// [typeconversion] created on 2025/5/10
float y;
int x;
int main() {
  y = 3.14;
  x = round(y);
  printf("%d\n", x);
  return 0;
}
