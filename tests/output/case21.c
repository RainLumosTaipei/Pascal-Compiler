#include <stdio.h>
#include <string.h>
/// [nestedcall] created on 2025/5/10
int doublevalue(int n) {
  int __doublevalue__;
  __doublevalue__ = n * 2;
  return __doublevalue__;
}
int add(int a, int b) {
  int __add__;
  __add__ = a + doublevalue(b);
  return __add__;
}
int main() {
  printf("%d\n", add(2, 3));
  return 0;
}
