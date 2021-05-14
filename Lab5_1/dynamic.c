//#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <math.h>
#define check(VALUE,BADVALUE,MSG) if (VALUE == BADVALUE){perror(MSG); exit(1); } 

int main(){
   char *libnames[] = {"./libfunctions1.so", "./libfunctions2.so"};
   int lib = 0;
   int (*GCF)(int, int) = NULL;
   float (*Square)(float, float) = NULL;
  
   void *handle;
   handle = dlopen(libnames[lib], RTLD_NOW);
   check(handle,NULL,dlerror());

   GCF = dlsym(handle,"GCF");
   check(GCF,NULL,dlerror());

   Square = dlsym(handle,"Square");
	check(Square,NULL,dlerror());

   int f,x,y;
   float a, b;
   while (scanf("%d", &f) > 0){
      if (f == 0){
         if (dlclose(handle) != 0)
            perror(dlerror());
         lib = (lib+1)%2;
         handle = dlopen(libnames[lib],RTLD_NOW);
         check(handle,NULL, dlerror());
         GCF = dlsym(handle,"GCF");
         check(GCF,NULL,dlerror());
         Square = dlsym(handle,"Square");
	      check(Square,NULL,dlerror());
      } 
      else if (f == 1){
         if (scanf("%d %d", &x, &y) == 2)
            printf("GCF(%d,%d) = %d\n", x, y, GCF(x,y));
      } 
      else if (f == 2){
         if(scanf("%f %f", &a, &b) == 2)
            printf("Square = %f\n", Square(a, b));
      }
      else
         printf("Incorrect comand!\n");
  }
  if (dlclose(handle) != 0)
     perror(dlerror());

}

