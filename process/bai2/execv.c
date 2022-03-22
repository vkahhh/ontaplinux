#include <unistd.h>
 
int main () 
{
    char *binaryPath = "/bin/ls";
    char *args[] = {binaryPath, "-lh", "/home", NULL};
 
    execv (binaryPath, args);
 
    return 0;
}