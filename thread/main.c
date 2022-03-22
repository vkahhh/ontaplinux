#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int signalsent = 0;
static pthread_mutex_t print_lock = PTHREAD_MUTEX_INITIALIZER;


static void pthread_pexit (int err, const char *fCall) 
{
	static void pexit (const char *fCall) 
	{ //in loi
		perror (fCall);
		exit (EXIT_FAILURE);
	}
	errno = err;
	pexit (fCall);
}

static void *print_pending_signals (__attribute__((unused)) void *arg) {
	int tid = *((int *) arg);

	/* cho main thread gui signal */
	while (!signalsent)
		sleep(1);

	/* in signal mask */
	sigset_t mask;
	int total = 0, sig, s;

	if (sigpending(&mask) == -1)
		pexit ("sigpending");

	/* sap xep dau ra cho 2 thread khong bi lon xon */
	s = pthread_mutex_lock (&print_lock);
	if (s != 0)
		pthread_pexit(s, "pthread_mutex_lock");

	printf ("Thread %d signal mask: ", tid);
	for (sig = 1; sig < NSIG; ++sig) {
		if (sigismember (&mask, sig)) {
			++total;
			printf ("%s ", strsignal (sig));
		}
	}

	if (total == 0)
		printf ("<empty>");

	printf ("\n");

	s = pthread_mutex_unlock (&print_lock);
	if (s != 0)
		pthread_pexit (s, "pthread_mutex_unlock");

	return NULL;
}


int main () {
	sigset_t blockset, oldset;
	int s;
	pthread_t t1, t2;
	int tid1, tid2;

	/* block SIGINT va SIGTERM trong main thread de cac luong tiep theo ke thu duoc signal mask */
	sigemptyset (&blockset);
	sigaddset (&blockset, SIGINT);
	sigaddset (&blockset, SIGTERM);
	s = pthread_sigmask (SIG_BLOCK, &blockset, &oldset);
	if (s != 0)
		pthread_pexit(s, "pthread_sigmask");

	/* tao cac thread */
	tid1 = 1;
	s = pthread_create (&t1, NULL, print_pending_signals, &tid1);
	if (s != 0)
		pthread_pexit (s, "pthread_create");

	tid2 = 2;
	s = pthread_create (&t2, NULL, print_pending_signals, &tid2);
	if (s != 0)
		pthread_pexit (s, "pthread_create");

	/* cac thread cho tin hieu duoc gui di */
	s = pthread_kill (t1, SIGINT);
	if (s != 0)
		pthread_pexit (s, "pthread_kill");

	s = pthread_kill (t2, SIGTERM);
	if (s != 0)
		pthread_pexit (s, "pthread_kill");

	/* thong bao voi thread rang signal da duoc gui */
	printf ("Main thread: signals were sent\n");
	signalsent = 1;

	/* join vaf exit thread*/
	s = pthread_joi (t1, NULL);
	if (s != 0)
		pthread_pexit (s, "pthread_join");

	s = pthread_join (t2, NULL);
	if (s != 0)
		pthread_pexit(s, "pthread_join");

	s = pthread_sigmask (SIG_SETMASK, &oldset, NULL);
	if (s != 0)
		pthread_pexit (s, "pthread_sigmask");

	exit(EXIT_SUCCESS);
}
