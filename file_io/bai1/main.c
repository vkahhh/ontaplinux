#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main () {
    int fd = open ("b1.txt", O_CREAT | O_WRONLY | O_APPEND);
    lseek (fd, 0, SEEK_SET);
    write (fd, "vkhanh", 6);
    close (fd);
}