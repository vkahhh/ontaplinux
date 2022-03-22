#include <stdio.h>
  
int main ()
{
    int* p = NULL;
  
    // This lines cause the segmentation 
    // error because of accessing the 
    // unknown memory location.
    *p = 1;
   
    printf ("%d", *p);

    return 0;
}