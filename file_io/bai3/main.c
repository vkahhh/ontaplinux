#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void main (int argc, char *argv[] )  
{  
  if ((argc < 5) || (argc > 5)) perror ("Argument = 5!\n");
  else 
  {
        char *file_name;
        file_name = argv [1];
        if (strcmp (argv [3], "r") == 0)
        {
            int fd = open (file_name, O_RDWR | O_CREAT, 0777);
            if (fd == -1)  perror ("open");
            char content [fd];
            int file_size = read (fd, content, atoi (argv [2]));
            if (file_size == -1)  perror ("read");
            printf("The number of bytes read is '%d'\n", file_size);
            close (fd);

        }
        else if (strcmp (argv [3], "w") == 0)
        {
        
            int fd = open (file_name, O_WRONLY | O_CREAT, 0644);
            if (fd == -1)  perror ("open"); 
            int t = write (fd, argv [4], atoi (argv [2]));
            if (t == -1)  perror ("write");
            printf ("%s\n", argv [4]);
            close (fd);
        }
        else
        {
            printf ("Error.\n");
        }

  } 

}  