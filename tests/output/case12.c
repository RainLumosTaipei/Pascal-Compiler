#include <stdio.h>
#include <string.h>
/// [functiondemo] created on 2025/5/10
int add(int a, int b) {
  int __add__;
  __add__ = a + b;
  return __add__;
}
int main() {
  printf("%d\n", add(3, 5));
  return 0;
}
