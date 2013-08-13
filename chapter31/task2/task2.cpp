
#include <iostream>
#include <cstring>

extern "C" {
#include "tlpi_hdr.h"
}

#include "functions.h"

const char * paths[8] = { "/ ", "/usr/bin/zip", "/etc/passwd////",
		"/etc////passwd", "passwd", "passwd/", ".. ", 0 };

char basenames[8][128];
char dirnames[8][128];

static void * threadFunc(void *arg) {
	int * j = (int *) arg;

	for (int i = *j; i < *j + 2; i++) {
		strcpy(basenames[i], my_basename(paths[i]));
		strcpy(dirnames[i], my_dirname(paths[i]));
	}

	return NULL;
}

int main() {

	pthread_t t[4];
	int args[4] = { 0, 2, 4, 6 };
	int s;
	for (int i = 0; i < 4; i++) {
		s = pthread_create(t + i, NULL, threadFunc, (void *) (args + i));
		if (s != 0)
			errExitEN(s, "pthread_create");
	}

	for (int i = 0; i < 4; i++) {
		s = pthread_join(t[i], NULL);
		if (s != 0)
			errExitEN(s, "pthread_join");
	}

	for (int i = 0; i < 7; i++) {
		std::cout << "Pathname:\t" << paths[i] << std::endl;
		std::cout << "dirname():\t" << dirnames[i] << std::endl;
		std::cout << "basename():\t" << basenames[i] << std::endl << std::endl;
	}

	std::cout << "Pathname:\t" << "NULL" << std::endl;
	std::cout << "dirname():\t" << dirnames[7] << std::endl;
	std::cout << "basename():\t" << basenames[7] << std::endl << std::endl;

	exit(EXIT_SUCCESS);

	return 0;
}
