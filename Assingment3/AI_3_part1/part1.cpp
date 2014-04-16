#include <iostream>
#include <stdio.h>
float alpha=0.3, x=0.7, y,prev_x ; 
int   iter=0 ; 
int count=0;

using namespace std;

float f(float x) { 
  return ((x)*(x))+(x+5) ; 
}

float derivative(float x) { 
  return 2*x + 1 ; 
}

void main(void) { 
  do { 
       iter++ ; 
       y = f(x) ; 
       printf ( "%5d x=%6.9f y=%6.3f\n", iter, x, y ) ; 
       x -= alpha * derivative(x) ;

  } while (iter < 50) ; 
  system("pause");
} 