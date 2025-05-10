#include <stdio.h>
#include <string.h>
/// [mixedfeatures] created on 2025/5/10
int arr[3];
int i;
int main() {
  for (i = 1; i <= 3; i++) {
    arr[i - 1] = i * 10;
  }
  if (arr[2 - 1] > 15) {
    printf("%d\n", arr[2 - 1]);
  } else {
    printf("%d\n", 0);
  }
  return 0;
}
