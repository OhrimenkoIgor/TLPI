
#include <cstring>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>

#include <pthread.h>

extern "C" {
#include "tlpi_hdr.h"
}

static const int BUFSIZE = 1024;

static pthread_once_t once = PTHREAD_ONCE_INIT;
static pthread_key_t strerrorKey;

static void /* Free thread-specific data buffer */
destructor(void *buf) {
	free(buf);
}

static void /* One-time key creation function */
createKey(void) {
	int s;
	/* Allocate a unique thread-specific data key and save the address
	 of the destructor for thread-specific data buffers */
	s = pthread_key_create(&strerrorKey, destructor);
	if (s != 0)
		errExitEN(s, "pthread_key_create");
}

static char * get_buffer() {
	int s;
	char *buf;
	/* Make first caller allocate key for thread-specific data */
	s = pthread_once(&once, createKey);
	if (s != 0)
		errExitEN(s, "pthread_once");
	buf = (char *) pthread_getspecific(strerrorKey);
	if (buf == NULL) { /* If first call from this thread, allocate
	 buffer for thread, and save its location */
		buf = (char *) malloc(BUFSIZE);
		if (buf == NULL)
			errExit("malloc");
		s = pthread_setspecific(strerrorKey, (void *) buf);
		if (s != 0)
			errExitEN(s, "pthread_setspecific");
	}
	return buf;
}

std::vector<std::string> &split(const std::string &s, char delim,
		std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	elems.erase(std::remove(elems.begin(), elems.end(), ""), elems.end());
	return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

char *my_dirname(const char *pathname) {
	char * buffer = get_buffer();
	if (!pathname) {
		strcpy(buffer, ".");
		return buffer;
	}
	std::string pn(pathname);
	pn.erase(std::remove(pn.begin(), pn.end(), ' '), pn.end());
	std::vector<std::string> v = split(pn, '/');
	if (v.size() == 0) {
		if (pn.find('/') != std::string::npos) {
			strcpy(buffer, "/");
		} else {
			strcpy(buffer, ".");
		}
	} else {
		std::string ret("");
		for (std::vector<std::string>::iterator it = v.begin();
				it != v.end() - 1; it++) {
			ret += "/";
			ret += *it;
		}
		if (ret == "")
			ret = ".";
		strcpy(buffer, ret.c_str());
	}
	return buffer;
}

char *my_basename(const char *pathname) {
	char * buffer = get_buffer();
	if (!pathname) {
		strcpy(buffer, ".");
		return buffer;
	}
	std::string pn(pathname);
	pn.erase(std::remove(pn.begin(), pn.end(), ' '), pn.end());
	std::vector<std::string> v = split(pn, '/');
	if (v.size() == 0) {
		if (pn.find('/') != std::string::npos) {
			strcpy(buffer, "/");
		} else {
			strcpy(buffer, ".");
		}
	} else {
		strcpy(buffer, (v.end() - 1)->c_str());
	}
	return buffer;
}

