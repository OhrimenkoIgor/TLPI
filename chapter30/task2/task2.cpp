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
#include <string>
#include <algorithm>

#include <pthread.h>

extern "C" {
#include "tlpi_hdr.h"
}

#include "tree.h"

using namespace std;

const int NUM = 26;
string keys[NUM] = { "apiary", "beetle", "cereal", "danger", "ensign", "florid",
		"garage", "health", "insult", "jackal", "keeper", "loaner", "manage",
		"nonce", "onset", "plaid", "quilt", "remote", "stolid", "train",
		"useful", "valid", "whence", "xenon", "yearn", "zippy" };

int values[NUM];

Tree tree;

static void * fillTreeFunc(void *arg) {

	int * j = (int *) arg;

	for (int i = *j; i < *j + NUM / 2; i++) {
		add(tree, keys[i].c_str(), &values[i]);
	}

	return NULL;
}

static void * lookupTreeFunc(void *arg) {

	int * j = (int *) arg;

	bool f;
	int count = 0;
	int * val;
	for (int i = *j; i < *j + NUM / 2; i++) {

		f = lookup(tree, keys[i].c_str(), (void **) &val);
		if (!f) {
			cout << "Can`t find key " << keys[i] << endl;
		} else {
			count++;
		}
	}

	return (void *) count;
}

static void * delTreeFunc(void *arg) {

	int * j = (int *) arg;

	for (int i = *j; i < *j + NUM / 2 - 3; i++) {
		del(tree, keys[i].c_str());
	}

	return NULL;
}

int main(int argc, char *argv[]) {

	int s;
	pthread_t t1, t2;

	for (int i = 0; i < NUM; i++) {
		values[i] = i;
	}

	initialize(tree);
	add(tree, keys[13].c_str(), 0);

	random_shuffle(keys, keys + NUM);

	//for (int i = 0; i < NUM; i++) {
	//	add(tree, keys[i].c_str(), &values[i]);
	//}

	int n1 = 0;
	int n2 = 13;

	 //fill tree
	 s = pthread_create(&t1, NULL, fillTreeFunc, &n1);
	 if (s != 0)
	 errExitEN(s, "pthread_create");
	 s = pthread_create(&t2, NULL, fillTreeFunc, &n2);
	 if (s != 0)
	 errExitEN(s, "pthread_create");

	 s = pthread_join(t1, NULL);
	 if (s != 0)
	 errExitEN(s, "pthread_join");
	 s = pthread_join(t2, NULL);
	 if (s != 0)
	 errExitEN(s, "pthread_join");
	 //end fill tree


	//lookup all elements
	int count1, count2 = 0;
	s = pthread_create(&t1, NULL, lookupTreeFunc, &n1);
	if (s != 0)
		errExitEN(s, "pthread_create");
	s = pthread_create(&t2, NULL, lookupTreeFunc, &n2);
	if (s != 0)
		errExitEN(s, "pthread_create");

	s = pthread_join(t1, (void **) &count1);
	if (s != 0)
		errExitEN(s, "pthread_join");
	s = pthread_join(t2, (void **) &count2);
	if (s != 0)
		errExitEN(s, "pthread_join");

	cout << "Found " << count1 + count2 << " nodes" << endl << endl;
	//end lookup tree

	//del(tree, "garage");
/*
	del(tree, keys[0].c_str());
	del(tree, keys[1].c_str());
	del(tree, keys[2].c_str());
	del(tree, keys[3].c_str());
	del(tree, keys[4].c_str());
	del(tree, keys[5].c_str());
	del(tree, keys[6].c_str());
	del(tree, keys[7].c_str());
	del(tree, keys[8].c_str());
	del(tree, keys[9].c_str());
*/
	//for (int i = 0; i < 0 + NUM / 2 - 3; i++) {
	//		del(tree, keys[i].c_str());
	//}


	 //delete 20 elements
	 cout << "Delete 20 elems" << endl;
	 s = pthread_create(&t1, NULL, delTreeFunc, &n1);
	 if (s != 0)
	 errExitEN(s, "pthread_create");
	 s = pthread_create(&t2, NULL, delTreeFunc, &n2);
	 if (s != 0)
	 errExitEN(s, "pthread_create");

	 s = pthread_join(t1, NULL);
	 if (s != 0)
	 errExitEN(s, "pthread_join");
	 s = pthread_join(t2, NULL);
	 if (s != 0)
	 errExitEN(s, "pthread_join");
	 //end delete 20 elements


	int count = 0;
	bool f;
	int * val;
	for (int i = 0; i < NUM; i++) {
		if (keys[i] != "garage") {
			f = lookup(tree, keys[i].c_str(), (void **) &val);
			if (f) {
				cout << keys[i] << "\t:\t" << *val << endl;
				count++;
			}
		}
	}
	cout << "Total " << count << endl;

	exit(EXIT_SUCCESS);

}
