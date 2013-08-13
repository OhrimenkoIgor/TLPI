#include <iostream>

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>
#include <signal.h>

extern "C" {
#include "tlpi_hdr.h"
}

using namespace std;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void * threadFunc1(void *arg) {

	sleep(1);
	sigset_t set, prevMask;

	sigemptyset(&set);
	sigaddset(&set, SIGCHLD);

	if (sigprocmask(SIG_BLOCK, &set, &prevMask) == -1)
		errExit("sigprocmask1");

	cout << "I am forking thread, TID = " << pthread_self() << endl;

	pid_t childPid;
	childPid = fork();
	if (childPid == -1)
		errExit("fork");
	if (childPid == 0) {/* Child */
		cout << "i am child " << getpid() << " of " << getppid() << endl;
		exit(0);
	}
	/* Parent waits for child to terminate */
	if (wait(NULL) == -1)
		errExit("wait");

	return NULL;
}

static void * threadFunc2(void *arg) {

	sigset_t set, prevMask;

	sigemptyset(&set);
	sigaddset(&set, SIGCHLD);

	if (sigprocmask(SIG_UNBLOCK, &set, &prevMask) == -1)
		errExit("sigprocmask1");

	sleep(2);

	return NULL;
}

void handler(int sig) {
	pthread_mutex_lock(&mutex);
	cout << "I am SIGCHLD handler, TID = " << pthread_self() << endl;
	pthread_mutex_unlock(&mutex);
}

int main() {
	pthread_t t1, t2;

	int s;

	cout << "i am main, PID = " << getpid() << endl;

	signal(SIGCHLD, handler);
	sigset_t set, prevMask;

	sigemptyset(&set);
	sigaddset(&set, SIGCHLD);

	if (sigprocmask(SIG_BLOCK, &set, &prevMask) == -1)
		errExit("sigprocmask1");

	s = pthread_create(&t1, NULL, threadFunc1, NULL);
	if (s != 0)
		errExitEN(s, "pthread_create");
	s = pthread_create(&t2, NULL, threadFunc2, NULL);
	if (s != 0)
		errExitEN(s, "pthread_create");

	s = pthread_join(t1, NULL);
	if (s != 0)
		errExitEN(s, "pthread_join");
	s = pthread_join(t2, NULL);
	if (s != 0)
		errExitEN(s, "pthread_join");

	exit(EXIT_SUCCESS);
}
