#include <stdio.h>
#include <string.h>
/// [expressionpriority] created on 2025/5/10
int x;
int main() {
  x = 3 + 5 * 2 % 4;
  printf("%d\n", x);
  return 0;
}
