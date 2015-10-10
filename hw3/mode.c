#include <stdio.h>
#include <stdlib.h>

int mode(int* arr, int len) {
  int currNum = arr[0];
  int count = 1;
  int mode = currNum;
  int modeCount = 0;
  int i = 1;
  while (i < len) {
    if (arr[i] == currNum) {
      count++;
    }
    if (i == len - 1 || arr[i] != currNum) {
      if (count > modeCount) {
        modeCount = count;
        mode = currNum;
      }
      currNum = arr[i];
      count = 1;
    }
    i++;
  }
  return mode;
}

int main(void) {
  int arr[] = {2, 2, 2, 3, 4, 6, 7, 9, 9, 9, 9};
  printf("%d\n", mode(arr, sizeof(arr) / sizeof(int)));
}
