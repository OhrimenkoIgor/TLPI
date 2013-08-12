
#include <pthread.h>

#include <iostream>

extern "C" {
#include "tlpi_hdr.h"
}

using namespace std;

struct Arg {
	int id;
	int loops;
};

static int glob = 0;
static void * /* Loop 'arg' times incrementing 'glob' */
threadFunc(void *arg) {
	Arg * a = (Arg *) arg;
	int loops = a->loops;
	int loc, j;
	for (j = 0; j < loops; j++) {
		cout << a->id << ":" << glob << endl;
		loc = glob;
		loc++;
		glob = loc;
	}
	return NULL;
}
int main(int argc, char *argv[]) {
	pthread_t t1, t2;
	Arg arg1, arg2;
	int s;
	arg1.loops = arg2.loops = (argc > 1) ? getInt(argv[1], GN_GT_0, "num-loops") : 10000000;
	arg1.id = 1;
	s = pthread_create(&t1, NULL, threadFunc, (void *) &arg1);
	if (s != 0)
		errExitEN(s, "pthread_create");
	arg2.id = 2;
	s = pthread_create(&t2, NULL, threadFunc, (void *) &arg2);
	if (s != 0)
		errExitEN(s, "pthread_create");
	s = pthread_join(t1, NULL);
	if (s != 0)
		errExitEN(s, "pthread_join");
	s = pthread_join(t2, NULL);
	if (s != 0)
		errExitEN(s, "pthread_join");
	printf("glob = %d\n", glob);
	exit(EXIT_SUCCESS);
}
