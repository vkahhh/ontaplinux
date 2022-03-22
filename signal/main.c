#include <stdio.h>
#include <signal.h>
#include <unistd.h>



int main ()
{
    sigset_t blockSet, oldMask;
 /* Khoi tao signal chua SIGINT */
    sigemptyset (&blockSet);
    sigaddset (&blockSet, SIGINT);
 /* Block SIGINT, luu lai thong tin cua signal mask cu */
    if (sigprocmask (SIG_BLOCK, &blockSet, &oldMask) == -1)
    {
        perror ("sigprocmask1");
    }
 /* In  thong tin signal mask*/
    printf ("%x", oldMask);

    return 0;
}