#include "tree.h"

TreeNode::TreeNode(const std::string & vkey, void * vvalue) :
		data(vkey, vvalue), left(0), right(0) {

}

TreeNode::~TreeNode() {

}

void initialize(Tree & tree) {
	tree.root = 0;
}

/*
 Если дерево пусто, заменить его на дерево с одним корневым узлом ((K,V), null, null) и остановиться.
 Иначе сравнить K с ключом корневого узла X.
 Если K>X, циклически добавить (K,V) в правое поддерево Т.
 Если K<X, циклически добавить (K,V) в левое поддерево Т.
 Если K=X, заменить V текущего узла новым значением. (хотя можно и организовать список значений V, но это другая тема)
 */

static void add(TreeNode* &tree, const char *key, void *value) {
	if (tree == 0) {
		tree = new TreeNode(key, value);
		return;
	}
	if (key > tree->data.key) {
		add(tree->right, key, value);
		return;
	}
	if (key < tree->data.key) {
		add(tree->left, key, value);
		return;
	}
	if (key == tree->data.key) {
		tree->data.value = value;
	}
}

void add(Tree & tree, const char *key, void *value) {
	add(tree.root, key, value);
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
	TreeNode * current_node = root;
	while (current_node->left) {
		current_node = current_node->left;
	}
	return current_node;
}

static void del(TreeNode* &tree, const char *key) {
	if (tree == 0) {
		return;
	}

	if (key < tree->data.key) {
		del(tree->left, key);
		return;
	}

	if (key > tree->data.key) {
		del(tree->right, key);
		return;
	}

	//if (key == tree->key){
	if (tree->left == 0 && tree->right == 0) {
		delete tree;
		tree = 0;
		return;
	}

	if (tree->left != 0 && tree->right == 0) {
		TreeNode * tmp = tree->right;
		delete tree;
		tree = tmp;
		return;
	}

	if (tree->left == 0 && tree->right != 0) {
		TreeNode * tmp = tree->left;
		delete tree;
		tree = tmp;
		return;
	}

	if (tree->left != 0 && tree->right != 0) {
		/*
		 if (tree->right->left == 0) {
		 TreeNode * tmp = tree->right;
		 tree->data = tree->right->data;
		 tree->right = tree->right->right;
		 delete tmp;
		 } else{
		 tree->data = tree->right->left->data;
		 del(tree->right->left, tree->right->left->data.key.c_str());
		 }
		 */

		TreeNode * successor = find_min(tree->right);
		tree->data = successor->data;
		del(successor, successor->data.key.c_str());

		return;
	}

	//} //if (key == tree->key)
}

void del(Tree & tree, const char *key) {
	del(tree.root, key);
}

/*
 Если дерево пусто, сообщить, что узел не найден, и остановиться.
 Иначе сравнить K со значением ключа корневого узла X.
 Если K=X, выдать ссылку на этот узел и остановиться.
 Если K>X, рекурсивно искать ключ K в правом поддереве Т.
 Если K<X, рекурсивно искать ключ K в левом поддереве Т.
 */

static bool lookup(const TreeNode * tree, const char *key, void **value) {
	if (!tree)
		return false;
	if (key == tree->data.key) {
		*value = tree->data.value;
		return true;
	}
	if (key < tree->data.key) {
		return lookup(tree->left, key, value);
	}
	if (key > tree->data.key) {
		return lookup(tree->right, key, value);
	}
	return false;
}

bool lookup(const Tree & tree, const char *key, void **value) {
	return lookup(tree.root, key, value);
}

