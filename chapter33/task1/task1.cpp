
#include <iostream>

#include <pthread.h>
#include <signal.h>

extern "C" {
#include "tlpi_hdr.h"
}

using namespace std;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void * threadFunc1(void *arg) {

	sigset_t set, prevMask;

	sigemptyset(&set);
	sigaddset(&set, SIGALRM);
	if (sigprocmask(SIG_BLOCK, &set, &prevMask) == -1)
		errExit("sigprocmask1");

	sleep(1);
	sigpending(&set);
	bool f = sigismember(&set, SIGALRM);

	pthread_mutex_lock(&mutex);
	cout << "Thread " << pthread_self() << " , SIGALRM pending: " << boolalpha << f << endl;
	pthread_mutex_unlock(&mutex);

	sleep(2);
	if (sigprocmask(SIG_UNBLOCK, &set, &prevMask) == -1)
			errExit("sigprocmask2");

	sleep(1);
	return NULL;
}

static void * threadFunc2(void *arg) {

	sigset_t set, prevMask;

	sigemptyset(&set);
	sigaddset(&set, SIGPWR);

	if (sigprocmask(SIG_BLOCK, &set, &prevMask) == -1)
		errExit("sigprocmask1");

	sleep(1);
	sigpending(&set);
	bool f = sigismember(&set, SIGPWR);
	pthread_mutex_lock(&mutex);
	cout << "Thread " << pthread_self() << " , SIGPWR pending: " << boolalpha << f << endl;
	pthread_mutex_unlock(&mutex);

	sleep(1);
	if (sigprocmask(SIG_UNBLOCK, &set, &prevMask) == -1)
			errExit("sigprocmask2");

	sleep(2);

	return NULL;
}

void handler(int sig) {
	pthread_mutex_lock(&mutex);
	cout << "I am signal handler" << endl;
	pthread_mutex_unlock(&mutex);
}

int main() {
	pthread_t t1, t2;

	int s;

	signal(SIGALRM, handler);
	signal(SIGPWR, handler);
	sigset_t set, prevMask;

	sigemptyset(&set);
	sigaddset(&set, SIGALRM);
	sigaddset(&set, SIGPWR);

	if (sigprocmask(SIG_BLOCK, &set, &prevMask) == -1)
		errExit("sigprocmask1");

	s = pthread_create(&t1, NULL, threadFunc1, NULL);
	if (s != 0)
		errExitEN(s, "pthread_create");
	s = pthread_create(&t2, NULL, threadFunc2, NULL);
	if (s != 0)
		errExitEN(s, "pthread_create");

	s = pthread_kill(t1, SIGALRM);
	if (s != 0)
		errExitEN(s, "pthread_kill");

	s = pthread_kill(t2, SIGPWR);
	if (s != 0)
		errExitEN(s, "pthread_kill");

	s = pthread_join(t1, NULL);
	if (s != 0)
		errExitEN(s, "pthread_join");
	s = pthread_join(t2, NULL);
	if (s != 0)
		errExitEN(s, "pthread_join");

	exit(EXIT_SUCCESS);
}
