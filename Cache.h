#ifndef CACHE_H
#define CACHE_H

#include "main.h"


class ReplacementPolicy {
	protected:
	int count;
	Elem** arr;
	
	public:
	Elem* removedElem = new Elem(0, 0, false);
	virtual int insert(Elem* e,int idx) = 0;//insert e into arr[idx] if idx != -1 else into the position by replacement policy
	virtual void access(int idx) = 0;//idx is index in the cache of the accessed element 
	virtual int remove() = 0; 
	virtual void print() = 0;
	virtual void replace(int idx, Elem* e) = 0;
	
	int getSize() { return count; }
	bool isFull() {return count == MAXSIZE;}
	bool isEmpty() {return count == 0;}
	int getAddr(int idx) { return arr[idx]->addr; }
	Elem* getValue(int idx){return arr[idx];}
};

class SearchEngine {
	public:
	virtual int search(int key) = 0; // -1 if not found
	virtual void insert(int key,int idx) = 0;
	virtual	void deleteNode(int key) = 0;
	virtual void print(ReplacementPolicy* r) = 0;
	virtual void replace(int key, int idx) = 0;
};

class FIFO : public ReplacementPolicy {
	public:
	int front;
	FIFO() { front = 0; count = 0; arr = new Elem * [MAXSIZE]; }
	~FIFO() { delete[]arr; }
	int insert(Elem* e, int idx);
	void access(int idx) { return; }
	int remove();
	void print();
	void replace(int idx, Elem* e) {
		/*delete arr[idx];
		access(idx);*/
		arr[idx] = e;
	}
};

class MRU : public ReplacementPolicy {
	public:
		MRU() { count = 0; arr = new Elem*[MAXSIZE]; }
		~MRU() { delete[]arr; }
	int insert(Elem* e, int idx);
	void access(int idx);
	int remove();
	void print();
	void replace(int idx, Elem* e) {
		//delete arr[idx];
		access(idx);
		arr[0] = e;
	}
};
class LRU: public MRU {
	public:
		int remove() override;
};

class LFU: public ReplacementPolicy {
	public:
		class newElem {
		public:
			Elem* elem;
			int freq;
			newElem(Elem* e) :elem(e), freq(0) {}
			~newElem() { delete elem; }
		};
		newElem** heap;
		LFU() { count = 0; arr = new Elem * [MAXSIZE]; heap = new newElem * [MAXSIZE]; }
		~LFU() { delete[]arr; delete[]heap; }
		int insert(Elem* e, int idx);
	void reheapDown(int heapSize);
	void reheapUp(int heapSize);
	void access(int idx);
	int remove();
	void print();
	void replace(int idx, Elem* e);
};

class DBHashing : public SearchEngine {
public:
	class Node {
	public:
		int key;
		int idx;
		Node(int key, int idx) { this->key = key; this->idx = idx; }
		~Node() {}
	};
	Node** hashTable;
	int curr_size;
	int size;
	int (*hash1)(int), (*hash2)(int);
	DBHashing(int (*h1)(int), int (*h2)(int), int s) {
		hashTable = new Node * [s];
		curr_size = 0;
		size = s;
		hash1 = h1;
		hash2 = h2;
		for (int i = 0; i < s; i++) {
			hashTable[i] = new Node(-1, -1);
		}
	}
	~DBHashing() { delete[]hashTable; }
	bool isFull() { return (curr_size == size); }
	void insert(int key, int i);
	void deleteNode(int key);
	void print(ReplacementPolicy* q);
	int search(int key);
	void replace(int key, int idx);

};
class AVL : public SearchEngine {
public:
	class Node {
	public:
		Node* left;
		Node* right;
		int height;
		int key;
		int idx;
		~Node() {
			left = NULL;
			right = NULL;
		}
	};
public:
	Node* root;
	AVL() {
		root = NULL;
	}
	~AVL() {
		deleteTree(root);
	}
	int max(int a, int b);
	int height(Node* N);
	Node* newNode(int key, int idx);
	Node* rightRotate(Node* y);
	Node* leftRotate(Node* x);
	int getBalance(Node* N);
	Node* insertNode(Node* node, int key, int idx);
	Node* minValueNode(Node* node);
	Node* deleteNodeAVL(Node* root, int key);
	void deleteTree(Node* node);
	int searchNode(Node* node, int addr);
	//Data* getData(Node* node, int addr);
	void replace(int key, int idx) { replaceIdx(root, key, idx); }
	void replaceIdx(Node* node, int addr, int idx);
	void printI(ReplacementPolicy* q, Node* root);
	void printP(ReplacementPolicy* q, Node* node);
	void insert(int key, int idx);
	void print(ReplacementPolicy* q);
	void deleteNode(int key);
	int search(int key);
};






#endif