#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
  
int main ()
{
    // open()tra ve file descriptor cua dup.txt
    int file_desc = open ("dup.txt", O_WRONLY | O_APPEND);
      
    if(file_desc < 0)
        printf ("Error opening the file\n");
      
    // dup() tao ban copy
  
    int copy_desc = dup (file_desc);
          
    // write(): viet chuoi vao file trong file descriptor 
  
    write (copy_desc,"This will be output to the file named dup.txt\n", 46);
          
    write (file_desc,"This will also be output to the file named dup.txt\n", 51);
      
    return 0;
}