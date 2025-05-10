#include <stdio.h>
#include <string.h>
/// [constantdemo] created on 2025/5/10
const float pi = 3.14;
float r;
int main() {
  r = 2.00;
  printf("%.2f\n", pi * r * r);
  return 0;
}
