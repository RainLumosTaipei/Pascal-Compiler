#include <stdio.h>
#include <string.h>
/// [setdemo] created on 2025/5/10
typedef int tnumbers[10];
tnumbers nums;
int main() {
  nums[1 - 1] = 1;
  nums[3 - 1] = 1;
  nums[5 - 1] = 1;
  if (nums[3 - 1]) {
    printf("%s\n", "3 is in the set");
  }
  return 0;
}
