#include <stdio.h>
#include <string.h>
/// [conditiondemo] created on 2025/5/10
int n;
int main() {
  n = -5;
  if (n > 0) {
    printf("%s\n", "Positive");
  } else {
    printf("%s\n", "Non-positive");
  }
  return 0;
}
