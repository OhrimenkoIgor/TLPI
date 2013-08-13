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

int main(int argc, char *argv[]) {

	const int NUM = 26;
	string keys[NUM] = { "apiary", "beetle", "cereal", "danger", "ensign",
			"florid", "garage", "health", "insult", "jackal", "keeper",
			"loaner", "manage", "nonce", "onset", "plaid", "quilt", "remote",
			"stolid", "train", "useful", "valid", "whence", "xenon", "yearn",
			"zippy" };

	int values[NUM];
	for (int i = 0; i < NUM; i++) {
		values[i] = i;
	}

	Tree tree;

	initialize(tree);

	add(tree, keys[13].c_str(), 0);

	random_shuffle(keys, keys + NUM);

	for (int i = 0; i < NUM; i++) {
		add(tree, keys[i].c_str(), &values[i]);
	}

	int * val;
	bool f = lookup(tree, "garage", (void **) &val);

	f = lookup(tree, "garade", (void **) &val);

	int nv = 30;
	add(tree, "garage", &nv);

	f = lookup(tree, "garage", (void **) &val);

	int count = 0;
	for (int i = 0; i < NUM; i++) {

		f = lookup(tree, keys[i].c_str(), (void **) &val);
		if (!f) {
			cout << keys[i] << endl;
		} else {
			count++;
		}

	}
	cout << count << endl;

	del(tree, "garage");

	count = 0;
	for (int i = 0; i < NUM; i++) {
		if (keys[i] != "garage") {
			f = lookup(tree, keys[i].c_str(), (void **) &val);
			if (!f) {
				cout << keys[i] << endl;
			} else {
				count++;
			}
		}
	}
	cout << count << endl;

	exit(EXIT_SUCCESS);
}
