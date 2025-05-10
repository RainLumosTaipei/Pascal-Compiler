#include <stdio.h>
#include <string.h>
/// [factorial] created on 2025/5/10
int fact(int n) {
  int __fact__;
  if (n <= 1) {
    __fact__ = 1;
  } else {
    __fact__ = n * fact(n - 1);
  }
  return __fact__;
}
int main() {
  printf("%d\n", fact(5));
  return 0;
}
