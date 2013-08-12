
#include "tree.h"

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

static void add(TreeNode* &tree, char *key, void *value) {
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

void add(Tree & tree, char *key, void *value) {
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
 Если левый узел m (6) правого поддерева отсутствует
 Копируем из (8) в (4) поля K, V и ссылку на правый узел.
 Иначе
 возьмем левый узел m (6), правого поддерева n->right (8);
 скопируем данные (кроме ссылок на дочерние элементы) из m (6) в n (4);
 рекурсивно удалим узел m (бывший 6).
 */

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
		if (tree->right->left == 0) {
			TreeNode * tmp = tree->right;
			tree->data = tree->right->data;
			tree->right = tree->right->right;
			delete tmp;
		} else{
			tree->data = tree->right->left->data;
			del(tree->right->left, tree->right->left->data.key.c_str());
		}

		return;
	}

	//} //if (key == tree->key)
}

void del(Tree & tree, char *key) {
	del(tree.root, key);
}

/*
 Если дерево пусто, сообщить, что узел не найден, и остановиться.
 Иначе сравнить K со значением ключа корневого узла X.
 Если K=X, выдать ссылку на этот узел и остановиться.
 Если K>X, рекурсивно искать ключ K в правом поддереве Т.
 Если K<X, рекурсивно искать ключ K в левом поддереве Т.
 */

static bool lookup(const TreeNode * tree, char *key, void **value) {
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

bool lookup(const Tree & tree, char *key, void **value) {
	return lookup(tree.root, key, value);
}

