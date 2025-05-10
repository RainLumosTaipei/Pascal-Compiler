#include <stdio.h>
#include <string.h>
/// [arraydemo] created on 2025/5/10
int arr[3];
int i;
int main() {
  for (i = 1; i <= 3; i++) {
    arr[i - 1] = i * 10;
  }
  for (i = 1; i <= 3; i++) {
    printf("%d\n", arr[i - 1]);
  }
  return 0;
}
