#include <stdio.h>
#include <string.h>
/// [repeatdemo] created on 2025/5/10
int i;
int main() {
  i = 0;
  do {
    i = i + 1;
  } while (!(i > 5));
  printf("%d\n", i);
  return 0;
}
