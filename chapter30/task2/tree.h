#ifndef TREE_H_
#define TREE_H_

#include <string>

#include <pthread.h>

struct Node {
	std::string key;
	void * value;
	Node(const std::string & vkey = "", void * vvalue = 0) :
			key(vkey), value(vvalue) {
	}
};

class Mutex {
	mutable pthread_mutex_t mutex;
public:
	Mutex();
	Mutex(const Mutex &) = delete;
	Mutex(Mutex &&) = delete;
	~Mutex();
	void lock() const;
	void unlock() const;
};

struct TreeNode: public Mutex {
	Node data;
	TreeNode * left;
	TreeNode * right;
	TreeNode(const std::string & vkey = "", void * vvalue = 0) :
			data(vkey, vvalue), left(0), right(0) {
	}
	TreeNode(const TreeNode &) = delete;
	TreeNode(TreeNode &&) = delete;
	~TreeNode() = default;

};

struct Tree: public Mutex {
	TreeNode * root;
};

void initialize(Tree & tree);
void add(Tree & tree, const char *key, void *value);
void del(Tree & tree, const char *key);
bool lookup(const Tree & tree, const char *key, void **value);

#endif /* TREE_H_ */
