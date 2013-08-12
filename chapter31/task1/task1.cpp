
#include <pthread.h>
#include <iostream>

extern "C" {
#include "tlpi_hdr.h"
}

using namespace std;

struct Control {
	bool f;
	pthread_mutex_t mutex;
} control = {
		false,
		PTHREAD_MUTEX_INITIALIZER
};

void one_time_init(Control * pcontrol, void (*init)(void)) {

	pthread_mutex_lock(&pcontrol->mutex);
	if(!pcontrol->f){
		init();
		pcontrol->f = true;
	}

	pthread_mutex_unlock(&pcontrol->mutex);
}

void ot_init(void){
	cout << "Hello, I am one time init" << endl;
}

static void * /* Loop 'arg' times incrementing 'glob' */
threadFunc(void *arg) {

	cout << "Hello, I am thread " << pthread_self() << endl;

	one_time_init(&control, ot_init);

	cout << "Bye, I am thread " << pthread_self() << endl;

	return NULL;
}

int main(int argc, char *argv[]) {
	pthread_t t1, t2;
	int s;
	s = pthread_create(&t1, NULL, threadFunc, 0);
	if (s != 0)
		errExitEN(s, "pthread_create");
	s = pthread_create(&t2, NULL, threadFunc, 0);
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
