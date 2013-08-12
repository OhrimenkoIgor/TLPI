
#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

static void *
threadFunc(void *arg) {
	struct someStruct *pbuf = (struct someStruct *) arg;
	/* Do some work with structure pointed to by 'pbuf' */
}
int main(int argc, char *argv[]) {
	struct someStruct buf;
	pthread_create(&thr, NULL, threadFunc, (void *) &buf);
	pthread_exit(NULL);
}

/*
Aside from the absence of error checking and various variable and structure
declarations, what is the problem with the following program?

No pthread_join() for created thread, so it will continue execution, as main thread called pthread_exit()
*/
