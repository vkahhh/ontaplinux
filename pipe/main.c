#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MSG_LEN 1024

static void childLoop (int readFd, int writeFd);
static void parentLoop (int readFd, int writeFd);
static void pexit (const char *fCall);

int main () 
{
	/* read and write pipes file descriptions, from the perspective of the parent process */
	int writePfd [2], readPfd [2];
	long childPid;

	/* create read and write pipes */
	if (pipe (writePfd) == -1)
		pexit ("pipe");

	if (pipe (readPfd) == -1)
		pexit ("pipe");

	childPid = fork ();

	switch (childPid) {
	case -1:
		pexit ("fork");
	case 0:
		/* child process: closes the write end of the write pipe, and the read end of the read pipe. */
		if (close (writePfd [1]) == -1)
			pexit ("close write end of write pipe - child");

		if (close (readPfd [0]) == -1)
			pexit ("close read end of read pipe - child");

		childLoop (writePfd [0], readPfd [1]);

		/* child loop is finished: this means that the parent process received EOF,
		 * and the write end of writePfd was closed */
		_exit (EXIT_SUCCESS);

	default:
		/* parent process: closes the read end of the write pipe, and the write end of the read pipe */
		if (close (writePfd [0]) == -1)
			pexit ("close read end of write pipe - parent");

		if (close (readPfd [1]) == -1)
			pexit ("close write end of read pipe - parent");

		parentLoop (readPfd [0], writePfd [1]);

		/* EOF received. Close write end of writePfd */
		if (close (writePfd [1]) == -1)
			pexit ("close write end of write pipe - parent");

		printf ("\n");
	}

	/* parent: wait for the child to die and terminate */
	wait (NULL);
	exit (EXIT_SUCCESS);
}

static void parentLoop (int readFd, int writeFd) 
{
	char buf [MSG_LEN];

	printf (">>");

	/* partial reads or EOF finish the parent loop */
	while (fgets (buf, MSG_LEN, stdin) != NULL) {

		/* remove the trailing newline character if present */
		if (buf [strlen(buf)-1] == '\n')
			buf [strlen(buf)-1] = '\0';

		/* write read string to writePdf, to be picked up by the child process */
		if (write (writeFd, buf, MSG_LEN) != MSG_LEN)
			pexit ("parent loop - partial write");

		/* wait the message from the child process */
		if (read (readFd, buf, MSG_LEN) != MSG_LEN)
			pexit ("parent loop - partial read");

		/* write what the child sent us back to the standard output */
		printf ("%s\n >>", buf);
	}
}

static void childLoop (int readFd, int writeFd) {
	char buf [MSG_LEN];
	unsigned int i;
	ssize_t numRead;

	/* wait for messages from the parent */
	while ((numRead = read (readFd, buf, MSG_LEN)) == MSG_LEN) {

		/* transform the incoming string to upper case */
		for (i = 0; i < strlen (buf); ++i) {
			buf [i] = toupper (buf [i]);
		}

		/* write the response back to the parent process */
		if (write (writeFd, buf, MSG_LEN) != MSG_LEN)
			pexit ("child loop - write failure");
	}

	if (numRead == -1)
		pexit ("child loop - read failure");
}

static void pexit (const char *fCall) {
	perror (fCall);
	exit (EXIT_FAILURE);
}