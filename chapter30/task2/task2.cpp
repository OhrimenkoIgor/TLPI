/*
 Implement a set of thread-safe functions that update and search an unbalanced
 binary tree. This library should include functions (with the obvious purposes) of
 the following form:

 initialize(tree);
 add(tree, char *key, void *value);
 delete(tree, char *key)
 Boolean lookup(char *key, void **value)

 In the above prototypes, tree is a structure that points to the root of the tree (you
 will need to define a suitable structure for this purpose). Each element of the tree
 holds a key-value pair. You will also need to define the structure for each element
 to include a mutex that protects that element so that only one thread at a time can
 access it. The initialize(), add(), and lookup() functions are relatively simple to implement.
 The delete() operation requires a little more effort.
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <map>

#include <pthread.h>

extern "C" {
#include "tlpi_hdr.h"
}

#include "tree.h"

using namespace std;

const int NUM = 10000;
const int NUM_THREADS = 10;
const int KEY_LEN = 15;

string keys[NUM] = { };
map<string, int*> nodes;
int arr[NUM] = { };

Tree tree;

string random_string(size_t length) {
	auto randchar = []() -> char
	{
		const char charset[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";
		const size_t max_index = (sizeof(charset) - 1);
		return charset[ rand() % max_index ];
	};
	string str(length, 0);
	generate_n(str.begin(), length, randchar);
	return str;
}

static void * fillTreeFunc(void *arg) {

	int * j = (int *) arg;

	for (int i = *j; i < *j + NUM / NUM_THREADS; i++) {
		do {
			keys[i] = random_string(rand() % KEY_LEN + 1);
		} while (nodes.find(keys[i]) != nodes.end());
		arr[i] = rand();
		nodes[keys[i]] = &arr[i];
		add(tree, keys[i].c_str(), &arr[i]);
	}

	return NULL;
}

static void * lookupTreeFunc(void *arg) {

	int * j = (int *) arg;

	bool f;
	int count = 0;
	int * val;
	for (int i = *j; i < *j + NUM / NUM_THREADS; i++) {

		f = lookup(tree, keys[i].c_str(), (void **) &val);
		if (!f) {
			cerr << "Can`t find key " << keys[i] << endl;
		} else if (val != nodes[keys[i]]) {
			cerr << "val != nodes[keys[i]]" << endl;
		} else {
			count++;
		}
	}

	return (void *) count;
}

static void * delTreeFunc(void *arg) {

	int * j = (int *) arg;

	for (int i = *j; i < *j + NUM / NUM_THREADS - 2; i++) {
		del(tree, keys[i].c_str());
	}

	return NULL;
}

int main(int argc, char *argv[]) {

	int s;
	pthread_t t[NUM_THREADS];
	int inds[NUM_THREADS];
	for (int i = 0; i < NUM_THREADS; i++) {
		inds[i] = i * (NUM / NUM_THREADS);
	}

	initialize(tree);

	//fill tree
	for (int i = 0; i < NUM_THREADS; i++) {
		s = pthread_create(t + i, NULL, fillTreeFunc, &inds[i]);
		if (s != 0)
			errExitEN(s, "pthread_create");
	}

	for (int i = 0; i < NUM_THREADS; i++) {
		s = pthread_join(t[i], NULL);
		if (s != 0)
			errExitEN(s, "pthread_join");
	}
	//end fill tree

	//lookup all elements
	int counts[NUM_THREADS];
	int count = 0;
	for (int i = 0; i < NUM_THREADS; i++) {
		s = pthread_create(t + i, NULL, lookupTreeFunc, &inds[i]);
		if (s != 0)
			errExitEN(s, "pthread_create");
	}

	for (int i = 0; i < NUM_THREADS; i++) {
		s = pthread_join(t[i], (void **) &counts[i]);
		if (s != 0)
			errExitEN(s, "pthread_join");
		count += counts[i];
	}
	cout << "Found " << count << " nodes" << endl << endl;
	//end lookup tree

	//delete 20 elements
	cout << "Delete N-20 elems" << endl;
	for (int i = 0; i < NUM_THREADS; i++) {
		s = pthread_create(t + i, NULL, delTreeFunc, &inds[i]);
		if (s != 0)
			errExitEN(s, "pthread_create");
	}

	for (int i = 0; i < NUM_THREADS; i++) {
		s = pthread_join(t[i], NULL);
		if (s != 0)
			errExitEN(s, "pthread_join");
	}
	//end delete 20 elements

	count = 0;
	bool f;
	int * val;
	for (int i = 0; i < NUM; i++) {
		if (keys[i] != "garage") {
			f = lookup(tree, keys[i].c_str(), (void **) &val);
			if (f) {
				cout.width(16);
				cout << keys[i] << "\t:\t" << *val << endl;
				count++;
			}
		}
	}
	cout << "Total " << count << endl;

	exit(EXIT_SUCCESS);

}
