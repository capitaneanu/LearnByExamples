#include <cstdio>
#include <iostream>
int globalvar = 20;

void do_something()
{
  int *i = nullptr ;
  *i = 5 ; 
}

int main()
{
   /* int arr[]{2,4,6,8,10};

    for(int i : arr)
    {
      printf("c=%d\n", i);
    }*/

   do_something();
  return 0 ;
}