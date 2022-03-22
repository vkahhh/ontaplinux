#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main ()
{
    int fd1 = open ("a.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    int fd2 = open ("a.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    int fd3 = open ("a.txt", O_RDWR);

    write (fd1, "Hello,", 6);
    write (fd2, "world", 6);
    lseek (fd2, 0, SEEK_SET);
    write (fd1, "HELLO,", 6);
    write (fd3, "Gidday", 6);

    return 0;
}

