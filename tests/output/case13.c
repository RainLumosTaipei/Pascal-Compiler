#include <stdio.h>
#include <string.h>
/// [recorddemo] created on 2025/5/10
typedef struct {
  int x, y;
} tpoint;
tpoint p;
int main() {
  p.x = 3;
  p.y = 4;
  printf("%c%d%c%d%c\n", '(', p.x, ',', p.y, ')');
  return 0;
}
