#include <stdio.h>
#include <string.h>
/// [scopetest] created on 2025/5/10
int x;
void modify() {
  int x;
  x = 10;
  printf("%d\n", x);
}
int main() {
  x = 5;
  modify();
  printf("%d\n", x);
  return 0;
}
