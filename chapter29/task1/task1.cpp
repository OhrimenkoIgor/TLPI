
#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#define handle_error_en(en, msg) \
        do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

int main(){

	int s = pthread_join(pthread_self(), NULL);

	if (s != 0)
		handle_error_en(s, "pthread_join");

	//if(tid != pthread_self())
	//pthread_join(tid, NULL);

	return 0;
}




