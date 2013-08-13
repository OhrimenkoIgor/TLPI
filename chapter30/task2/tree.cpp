extern "C" {
#include "tlpi_hdr.h"
}

#include "tree.h"

Mutex::Mutex() {
	int s = pthread_mutex_init(&mutex, NULL);
	if (s != 0)
		errExitEN(s, "pthread_mutex_init");
}

Mutex::~Mutex() {
	int s = pthread_mutex_destroy(&mutex); /* No longer needed */
	if (s != 0)
		errExitEN(s, "pthread_mutex_destroy");
}

void Mutex::lock() const {
	int s = pthread_mutex_lock(&mutex);
	if (s != 0)
		errExitEN(s, "pthread_mutex_lock");
}

void Mutex::unlock() const {
	int s = pthread_mutex_unlock(&mutex);
	if (s != 0)
		errExitEN(s, "pthread_mutex_unlock");
}

void initialize(Tree & tree) {
	tree.lock();
	tree.root = 0;
	tree.unlock();
}

/*
 Если дерево пусто, заменить его на дерево с одним корневым узлом ((K,V), null, null) и остановиться.
 Иначе сравнить K с ключом корневого узла X.
 Если K>X, циклически добавить (K,V) в правое поддерево Т.
 Если K<X, циклически добавить (K,V) в левое поддерево Т.
 Если K=X, заменить V текущего узла новым значением. (хотя можно и организовать список значений V, но это другая тема)
 */

static void add(TreeNode* &tree, const char *key, void *value,
		Mutex* caller) {

	if (tree == 0) {
		tree = new TreeNode(key, value);
		caller->unlock();
		return;
	}
	tree->lock();
	caller->unlock();
	if (key > tree->data.key) {
		add(tree->right, key, value, tree);
		return;
	}
	if (key < tree->data.key) {
		add(tree->left, key, value, tree);
		return;
	}
	if (key == tree->data.key) {
		tree->data.value = value;
	}
	tree->unlock();
}

void add(Tree & tree, const char *key, void *value) {
	tree.lock();
	add(tree.root, key, value, &tree);
}

/*
 Если дерево T пусто, остановиться;
 Иначе сравнить K с ключом X корневого узла n.
 Если K>X, циклически удалить K из правого поддерева Т;
 Если K<X, циклически удалить K из левого поддерева Т;
 Если K=X, то необходимо рассмотреть три случая.

 Если обоих детей нет, то удаляем текущий узел и обнуляем ссылку на него у родительского узла;
 Если одного из детей нет, то значения полей ребёнка m ставим вместо соответствующих значений корневого узла,
 затирая его старые значения, и освобождаем память, занимаемую узлом m;
 Если оба ребёнка присутствуют, то
 Call the node to be deleted N. Do not delete N. Instead, choose either its in-order successor node or its in-order predecessor node, R.
 Replace the value of N with the value of R, then delete R.
 As with all binary trees, a node's in-order successor is the left-most child of its right subtree,
 and a node's in-order predecessor is the right-most child of its left subtree. In either case, this node will have zero or one children.
 Delete it according to one of the two simpler cases above.
 */

static TreeNode * find_min(TreeNode * root) { //Gets minimum node (leftmost leaf) in a subtree
	root->lock();
	TreeNode * current_node = root;
	TreeNode * next_node = current_node->left;
	while (next_node) {
		next_node->lock();
		current_node->unlock();
		current_node = next_node;
		next_node = current_node->left;
	}
	return current_node;
}

static void del(TreeNode* &tree, const char *key, Mutex* caller) {
	if (tree == 0) {
		caller->unlock();
		return;
	}


	if (key == tree->data.key && tree->left == 0 && tree->right == 0) {
		delete tree;
		tree = 0;
		caller->unlock();
		return;
	}

	if (key == tree->data.key && tree->left != 0 && tree->right == 0) {
		TreeNode * tmp = tree->right;
		delete tree;
		tree = tmp;
		caller->unlock();
		return;
	}

	if (key == tree->data.key && tree->left == 0 && tree->right != 0) {
		TreeNode * tmp = tree->left;
		delete tree;
		tree = tmp;
		caller->unlock();
		return;
	}

	tree->lock();
	caller->unlock();

	if (key < tree->data.key) {
		del(tree->left, key, tree);
		return;
	}

	if (key > tree->data.key) {
		del(tree->right, key, tree);
		return;
	}

	if (key == tree->data.key && tree->left != 0 && tree->right != 0) {

		TreeNode * successor = find_min(tree->right);
		tree->data = successor->data;
		successor->unlock(); //after lock() in find_min(tree->right);
		del(successor, successor->data.key.c_str(), tree);

		return;
	}

}

void del(Tree & tree, const char *key) {
	tree.lock();
	del(tree.root, key, &tree);
}

/*
 Если дерево пусто, сообщить, что узел не найден, и остановиться.
 Иначе сравнить K со значением ключа корневого узла X.
 Если K=X, выдать ссылку на этот узел и остановиться.
 Если K>X, рекурсивно искать ключ K в правом поддереве Т.
 Если K<X, рекурсивно искать ключ K в левом поддереве Т.
 */

static bool lookup(const TreeNode * tree, const char *key, void **value, const Mutex* caller) {
	if (!tree){
		caller->unlock();
		return false;
	}
	tree->lock();
	caller->unlock();
	if (key == tree->data.key) {
		*value = tree->data.value;
		tree->unlock();
		return true;
	}
	if (key < tree->data.key) {
		return lookup(tree->left, key, value, tree);
	}
	if (key > tree->data.key) {
		return lookup(tree->right, key, value, tree);
	}
	tree->unlock();
	return false;
}

bool lookup(const Tree & tree, const char *key, void **value) {
	tree.lock();
	return lookup(tree.root, key, value, &tree);
}

