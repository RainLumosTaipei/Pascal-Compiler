#include <stdio.h>
#include <string.h>
/// [whiledemo] created on 2025/5/10
int i;
int main() {
  i = 0;
  while (i < 3) {
    i = i + 1;
  }
  printf("%d\n", i);
  return 0;
}
