#include "Cache.h"
Cache::Cache(SearchEngine* s,ReplacementPolicy* r):rp(r),s_engine(s) {}
// ---------------------------------------------AVL Tree-----------------------------------------------------
int AVL::max(int a, int b) {
    return (a > b) ? a : b;
}
int AVL::height(Node* N) {
	if (N == NULL) {
		return 0;
	}
	return N->height;
}
AVL::Node* AVL::newNode(int key, int idx) {
	Node* node = new Node();
	node->key = key;
	node->idx = idx;
	node->left = NULL;
	node->right = NULL;
	node->height = 1;

	return node;
}
AVL::Node* AVL::rightRotate(Node* y) {
	Node* x = y->left;
	Node* T2 = x->right;

	// Rotate
	x->right = y;
	y->left = T2;

	// Update heights
	y->height = max(height(y->left), height(y->right)) + 1;
	x->height = max(height(x->left), height(x->right)) + 1;

	return x;
}
AVL::Node* AVL::leftRotate(Node* x) {
	Node* y = x->right;
	Node* T2 = y->left;

	// Rotate
	y->left = x;
	x->right = T2;

	// Update heights
	x->height = max(height(x->left), height(x->right)) + 1;
	y->height = max(height(y->left), height(y->right)) + 1;

	return y;
}
int AVL::getBalance(Node* N) {
	if (N == NULL) {
		return 0;
	}
	return height(N->left) - height(N->right);
}
AVL::Node* AVL::insertNode(Node* node, int key, int idx) {
	if (node == NULL) {
		return newNode(key, idx);
	}
	if (key < node->key) {
		node->left = insertNode(node->left, key, idx);	// Insert to the left
	}
	else if (key > node->key) {
		node->right = insertNode(node->right, key, idx);	// Insert to the right
	}
	else return node;

	node->height = 1 + max(height(node->left), height(node->right));

	int balance = getBalance(node);

	if (balance > 1 && key < node->left->key) { // Left left case
		return rightRotate(node);
	}
	if (balance < -1 && key > node->right->key) { // Right right case
		return leftRotate(node);
	}
	if (balance > 1 && key > node->left->key) {	// Left right case
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}
	if (balance < -1 && key < node->right->key) { // Right left case
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}
	return node;
}
AVL::Node* AVL::minValueNode(Node* node) {
	Node* current = node;

	while (current->left != NULL) {
		current = current->left;
	}

	return current;
}
AVL::Node* AVL::deleteNodeAVL(Node* root, int key) {
	if (root == NULL) {
		return root;
	}
	if (key < root->key) {
		root->left = deleteNodeAVL(root->left, key);
	}
	else if (key > root->key) {
		root->right = deleteNodeAVL(root->right, key);
	}
	else {
		if ((root->left == NULL) || (root->right == NULL)) {
			Node* temp = (root->left) ? root->left : root->right;

			if (temp == NULL) { // No child
				temp = root;
				root = NULL;
			}
			else {
				*root = *temp;
			}
			free(temp);
		}
		else {
			Node* temp = minValueNode(root->right);

			root->key = temp->key;

			root->right = deleteNodeAVL(root->right, temp->key);
		}
	}
	if (root == NULL) {
		return root;
	}

	root->height = 1 + max(height(root->left), height(root->right));

	int balance = getBalance(root);

	if (balance > 1 && getBalance(root->left) >= 0) { // Left left case
		return rightRotate(root);
	}
	if (balance < -1 && getBalance(root->right) <= 0) { // Right right case
		return leftRotate(root);
	}
	if (balance > 1 && getBalance(root->left) < 0) {	// Left right case
		root->left = leftRotate(root->left);
		return rightRotate(root);
	}
	if (balance < -1 && getBalance(root->right) > 0) { // Right left case
		root->right = rightRotate(root->right);
		return leftRotate(root);
	}
	return root;
}
void AVL::deleteTree(Node* node)
{
	if (node == NULL) return;

	/* first delete both subtrees */
	deleteTree(node->left);
	deleteTree(node->right);

	/* then delete the node */
	delete node;
}
int AVL::searchNode(Node* node, int key) {
	if (node != NULL) {
		if (node->key == key) {
			return node->idx;
		}
		else if (key < node->key) {
			return searchNode(node->left, key);
		}
		else {
			return searchNode(node->right, key);
		}
	}
	return -1;
}
int AVL::search(int key) { return searchNode(root, key); }
void AVL::insert(int key, int idx) { root = insertNode(root, key, idx); }
void AVL::deleteNode(int key) { root = deleteNodeAVL(root, key); }
void AVL::replaceIdx(Node* node, int addr, int idx) {
	if (node != NULL) {
		if (node->key == addr) {
			if (node->idx != idx) {
				node->idx = idx;
				return;
			}
		}
		else if (addr < node->key) {
			replaceIdx(node->left, addr, idx);
		}
		else {
			replaceIdx(node->right, addr, idx);
		}
	}
}
// ------------------------------------------------FIFO--------------------------------------------------------
int FIFO::insert(Elem* e, int idx) {
	if (idx == -1) {
		arr[count] = e;
		return count++;
	}
	else {
		arr[idx] = e;
		count++;
		return idx;
	}
}
int FIFO::remove() {
	Elem* t = new Elem(0, 0, false);
	if (count < 1) return -1;
	removedElem = arr[0];
	if (count > 1) {
		for (int i = 0; i < count - 1; i++) {
			arr[i] = arr[i + 1];
		}
		arr[count - 1] = NULL;
	}
	else {
		arr[0] = NULL;
	}
	count--;
	int key = removedElem->addr; // might add delete t;
	return key;
}
void FIFO::print() {
	for (int i = 0; i < count; ++i) {
		arr[i]->print();
	}
}
// ------------------------------------------------Cache---------------------------------------------------------------
Cache::~Cache(){
	delete rp;
	delete s_engine;
}
Data* Cache::read(int addr) {
	int idx = s_engine->search(addr);
	if (idx > -1) {
		rp->access(idx);
		int newIdx = idx;
		for (int i = 0; i < rp->getSize(); i++) {
			if (rp->getAddr(i) == addr) {
				newIdx = i;
				break;
			}
		}
		if (newIdx != idx) { // if access changes the idx, aka not FIFO
			for (int i = 0; i < rp->getSize(); i++) {	// Update the idx for each element in search engine
				s_engine->replace(rp->getAddr(i), i);
			}
		}
		return rp->getValue(newIdx)->data;
	}
	return NULL;
}
Elem* Cache::put(int addr, Data* cont) {
	Elem* temp = new Elem(addr, cont, true);
	if (rp->isFull()) {
		int res = rp->remove(); // key of removedElem
		s_engine->deleteNode(res); // remove Node in engine
		int idx = rp->insert(temp, -1); // 
		s_engine->insert(addr, idx);
		for (int i = 0; i < rp->getSize(); i++) {
			s_engine->replace(rp->getAddr(i), i);
		}
		return rp->removedElem;
	}
	else {
		int idx = rp->insert(temp, -1);
		s_engine->insert(addr, idx);
		for (int i = 0; i < rp->getSize(); i++) {
			s_engine->replace(rp->getAddr(i), i);
		}
		return NULL;
	}
}
Elem* Cache::write(int addr, Data* cont) {
	bool found = false;
	int idx = s_engine->search(addr);
	Elem* temp = new Elem(addr, cont, false);
	if (idx != -1) {
		//rp->access(idx);
		rp->replace(idx, temp);
		for (int i = 0; i < rp->getSize(); i++) {
			s_engine->replace(rp->getAddr(i), i);
		}
		found = true;
		return NULL;
	}
	if (!found) {
		if (rp->isFull()) {
			int res = rp->remove();
			s_engine->deleteNode(res);
			int newIdx = rp->insert(temp, -1);
			s_engine->insert(addr, newIdx);
			for (int i = 0; i < rp->getSize(); i++) {
				s_engine->replace(rp->getAddr(i), i);
			}
			return rp->removedElem;
		}
		else {
			int idx = rp->insert(temp, -1);
			s_engine->insert(addr, idx);
			for (int i = 0; i < rp->getSize(); i++) {
				s_engine->replace(rp->getAddr(i), i);
			}
			return NULL;
		}
	}
    return NULL;
}
// ------------------------------------------------MRU/LRU--------------------------------------------------------
int MRU::insert(Elem* e, int idx) {
	if (idx == -1) {
		for (int i = count; i > 0; i--) {
			arr[i] = arr[i - 1];
		}
		arr[0] = e;
		count++;
		return 0;
	}
	arr[idx] = e;
	count++;
	return idx;
}
int MRU::remove() {
	if (count < 1) return -1;
	count--;
	removedElem = arr[0];
	for (int i = 0; i <= count - 1; i++) {
		arr[i] = arr[i + 1];
	}
	arr[count] = NULL;
	return removedElem->addr;
}
void MRU::access(int idx) {
	if (idx >= count) return;
	Elem* temp = arr[idx];
	for (int i = idx; i > 0; i--) {
		arr[i] = arr[i - 1];
	}
	arr[0] = temp;
}
void MRU::print() {
	for (int i = 0; i < count; ++i) {
		arr[i]->print();
	}
}
int LRU::remove() {
	if (count < 1) return -1;
	count--;
	removedElem = arr[count];
	arr[count] = NULL;
	return removedElem->addr;
}
// ------------------------------------------------LFU--------------------------------------------------------
int LFU::insert(Elem* e, int idx) {
	if (idx == -1) {
		arr[count] = e;
		heap[count] = new newElem(e);
		heap[count]->freq++;
		++count;	
		reheapUp(count);
		for (int i = 0; i < count; i++) {
			arr[i] = heap[i]->elem;
		}
		for (int i = 0; i < count; i++) {
			if (arr[i]->addr == e->addr) {
				return i; // index of element
			}
		}
	}
	return -1;
}
void LFU::reheapDown(int idx) {
	int i = idx;
	newElem* temp = heap[i];
	while (2 * i + 2 < getSize()) {
		if ((heap[i]->freq >= heap[2 * i + 1]->freq) && (heap[2 * i + 1]->freq > heap[2 * i + 2]->freq)) {
			temp = heap[i];
			heap[i] = heap[2 * i + 2];
			heap[2 * i + 2] = temp;
			i = 2 * i + 2;
		}
		else if (heap[i]->freq >= heap[2 * i + 1]->freq) {
			temp = heap[i];
			heap[i] = heap[2 * i + 1];
			heap[2 * i + 1] = temp;
			i = 2 * i + 1;
		}
		else {
			//free(temp);
			break; 
		}
	}
	if (2 * i + 1 < getSize()) {
		if (heap[i]->freq >= heap[2 * i + 1]->freq) {
			temp = heap[i];
			heap[i] = heap[2 * i + 1];
			heap[2 * i + 1] = temp;
		}
	}
}
void LFU::reheapUp(int heapSize) {
	int i = heapSize - 1;
	newElem* temp = heap[0];
	while ((i - 1) / 2 >= 0) {
		if (heap[i]->freq < heap[(i - 1) / 2]->freq) {
			temp = heap[i];
			heap[i] = heap[(i - 1) / 2];
			heap[(i - 1) / 2] = temp;
			i = (i - 1) / 2;
		}
		else {
			//free(temp);
			break;
		}
	}
}
void LFU::access(int idx) {
	heap[idx]->freq++;
	reheapDown(idx);
	for (int i = 0; i < count; i++) {
		arr[i] = heap[i]->elem;
	}
}
int LFU::remove() {
	if (count < 1) return -1;
	count--;
	newElem* temp = heap[0];	// swap root and furthest child
	heap[0] = heap[count];	
	heap[count] = temp;
	removedElem = temp->elem;	// remove the swapped element (previously root)
	reheapDown(0); // count = new size after removed
	for (int i = 0; i < count; i++) {
		arr[i] = heap[i]->elem;
	}
	free(temp);
	return removedElem->addr;
}
void LFU::print() {
	for (int i = 0; i < count; i++) {
		arr[i]->print();
	}
}
void LFU::replace(int idx, Elem* e) {
	access(idx);
	for (int i = 0; i < count; i++) {
		if (arr[i]->addr == e->addr) {
			arr[i] = e;
			heap[i]->elem = e;
		}
	}
}
// ------------------------------------------------DbHash--------------------------------------------------------
void DBHashing::insert(int key, int i) {
	if (isFull()) return;

	int index = hash1(key) % size;

	if (hashTable[index]->key != -1) {
		int index2 = hash2(key) % size;
		int j = 1;
		while (true) {
			int newIndex = (index + j * index2) % size;

			if (hashTable[newIndex]->key == -1) {
				hashTable[newIndex]->idx = i;
				hashTable[newIndex]->key = key;
				break;
			}
			j++;
		}
	}
	else {
		hashTable[index]->key = key;
		hashTable[index]->idx = i;
	}
	curr_size++;
}
void DBHashing::deleteNode(int key) {
	for (int i = 0; i < size; i++) {
		if (hashTable[i]->key != -1) {
			cout << hashTable[i]->key << " ";
		}
	}
	cout << endl;
	for (int i = 0; i < size; i++) {
		if (hashTable[i]->key == key) {
			hashTable[i]->key = -1;
			hashTable[i]->idx = -1;
			break;
		}
	}
	curr_size--;
}
int DBHashing::search(int key) {
	for (int i = 0; i < size; i++) {
		if (hashTable[i]->key == key) {
			return hashTable[i]->idx;
		}
	}
	return -1;
}
void DBHashing::replace(int key, int idx) {
	for (int i = 0; i < size; i++) {
		if (hashTable[i]->key == key) {
			hashTable[i]->idx = idx;
			break;
		}
	}
}
void DBHashing::print(ReplacementPolicy* q) {
	for (int i = 0; i < size; i++) {
		if (hashTable[i]->key != -1) {
			q->getValue(hashTable[i]->idx)->print();
		}
	}
}
void AVL::printI(ReplacementPolicy* q, Node* node) {
	if (node != NULL) {
		printI(q, node->left);
		q->getValue(node->idx)->print();
		printI(q, node->right);
	}
}
void AVL::printP(ReplacementPolicy* q, Node* node) {
	if (node != NULL) {
		q->getValue(node->idx)->print();
		printP(q, node->left);
		printP(q, node->right);
	}
}
void AVL::print(ReplacementPolicy* q) {
	cout << "Print AVL in inorder:\n";
	printI(q, root);
	cout << "Print AVL in preorder:\n";
	printP(q, root);
}
void Cache::printRP() {
	rp->print();
}
void Cache::printSE() {
	s_engine->print(rp);
}
