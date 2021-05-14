#include "functions.h"
#include <stdio.h>
#include <stdlib.h>


int main(){
    int c, x, y;
    float a, b;
    while (scanf("%d", &c) > 0){
          if (c == 1){
            if(scanf("%d %d", &x, &y) == 2)
		      printf("GCF(%d %d) = %d\n", x, y, GCF(x,y));
          } 
          else if (c == 2){ 
            if(scanf("%f %f", &a, &b) == 2)
                  printf("Square = %f\n", Square(a, b));
          }
    }
}

