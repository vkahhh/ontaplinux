#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
  
int main ()
{
    int file_desc = open ("dup2.txt",O_WRONLY | O_APPEND);
      
    // tao newfd trong file descriptor stdout
    dup2 (file_desc, 1) ; 
          
    // in ra toan bo statement
    printf ("I will be printed in the file dup2.txt\n");
      
return 0;
}