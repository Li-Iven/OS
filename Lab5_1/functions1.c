#include "functions.h"

float Square(float a, float b){
    return a*b;
}

int GCF(int x, int y){
  while (x != y){
    if (x>=y)
      x = x - y;
    else 
      y = y - x;
  }
  return x;
}

