#include "functions.h"

float Square(float a, float b){
    return a*b/2;
}

void Swap(int* x, int* y) {
        int tmp = *x;
        *x = *y;
        *y = tmp;
}

int GCF(int x, int y){
  if (x>y)
     Swap(&x,&y);
  for (int i = x; i>1 ; i--){
     if (x % i == 0 && y % i == 0)
        return i;
  }
  return 1;
}
