//Name = Navjot Singh		//Date = 2024-11-22		//student id: 301609090

#pragma once
#include <string>
#include <iostream>
#include <fstream> //files
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::ofstream;

class AVLTreeNode
{
public:
	AVLTreeNode *parent;
	AVLTreeNode *left;
	AVLTreeNode *right;
	string word;
	int count;
	int height;

	AVLTreeNode(string);
	AVLTreeNode(string, AVLTreeNode *, AVLTreeNode *, AVLTreeNode *, unsigned int, unsigned int);
};

// Wordlist class
class Wordlist
{
private:
	// Your class MUST have a root node named root (below)
	AVLTreeNode *root; // DO NOT REMOVE
	unsigned int size; 

	void inOrderTraversal(AVLTreeNode *root, int &counter) const;

	// HELPERS
	AVLTreeNode *traversalCopy(AVLTreeNode *copy, AVLTreeNode *parent);
	void postOrderTraversalDeletion(AVLTreeNode *n);
	int maxInt(const int &a, const int &b);
	int getHeight(AVLTreeNode *node);
	void updateHeight(AVLTreeNode *node);
	int getBalanceFactor(AVLTreeNode *node);
	void rotateLeft(AVLTreeNode *x);
	void rotateRight(AVLTreeNode *y);
	void balance(AVLTreeNode *node, string insertedWord);
	AVLTreeNode *insertionHelp(AVLTreeNode *root, AVLTreeNode *newNode);
	void traverseIncrement(AVLTreeNode *root, const string &word);
	int count(AVLTreeNode *root, const string &word);
	int totalNumberOfWords(AVLTreeNode *root, int count) const;
	AVLTreeNode *max(AVLTreeNode *a, AVLTreeNode *b) const;
	AVLTreeNode *mostFreq(AVLTreeNode *root) const;
	int singletonCount(AVLTreeNode *root, int &count) const;
	AVLTreeNode *inOrderPredecessor(AVLTreeNode *root);
	AVLTreeNode *deleteNode(AVLTreeNode *root, string word);

public:
	// public methods go here
	Wordlist();
	Wordlist(const string &);
	Wordlist(const Wordlist &);
	~Wordlist();

	Wordlist &operator=(const Wordlist &);
	void insert(const string &);
	bool remove(const string &);
	int getCount(string);
	bool contains(string);
	int differentWords() const;
	int totalWords() const;
	string mostFrequent() const;
	int singletons() const;
	void printWords() const;
	// Prints useful statistics about the word list
	void printStatistics() const;

	// Returns the root of the AVL tree
	AVLTreeNode *getRoot() const { return root; }; // DO NOT REMOVE
};