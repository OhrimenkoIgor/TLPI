
#ifndef TREE_H_
#define TREE_H_

#include <string>

struct Node {
	std::string key;
	void * value;
	Node (const std::string & vkey = "",  void * vvalue = 0) : key(vkey), value(vvalue) {}
};

struct TreeNode {
	Node data;
	TreeNode * left;
	TreeNode * right;
	TreeNode(const std::string & vkey = "",  void * vvalue = 0);
	~TreeNode();
};

struct Tree {
	TreeNode * root;
};

void initialize(Tree & tree);
void add(Tree & tree, const char *key, void *value);
void del(Tree & tree, const char *key);
bool lookup(const Tree & tree, const char *key, void **value);


#endif /* TREE_H_ */
