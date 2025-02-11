//Name = Navjot Singh		//Date = 2024-11-22		//student id: 301609090


#include "Wordlist.h"
#include <iomanip>
#include <stdexcept>
using std::fixed;
using std::setprecision;
using std::invalid_argument;


// Prints useful statistics about the word list

AVLTreeNode::AVLTreeNode(string word)
{
	this->parent = nullptr;
	this->left = nullptr;
	this->right = nullptr;
	this->word = word;
	this->count = 0;
	this->height = 0;
}
AVLTreeNode::AVLTreeNode(string word, AVLTreeNode *parent, AVLTreeNode *left, AVLTreeNode *right, unsigned int count, unsigned int height)
{
	this->parent = parent;
	this->left = left;
	this->right = right;
	this->word = word;
	this->count = count;
	this->height = height;
}
Wordlist::Wordlist()
{
	root = nullptr;
	this->size=0;
}

Wordlist::Wordlist(const Wordlist &copy)
{
	this->root = traversalCopy(copy.root, nullptr);
	this->size = copy.size;
}

Wordlist::~Wordlist()
{
	postOrderTraversalDeletion(this->root);
	this->size = 0;
	this->root = nullptr;

}

Wordlist &Wordlist::operator=(const Wordlist &copy)
{
	if (this != &copy)
	{													// Check for self-assignment
		postOrderTraversalDeletion(this->root);			// Clear existing tree
		this->root = traversalCopy(copy.root, nullptr); // Deep copy
		this->size = copy.size;
	}
	return *this;
}
Wordlist::Wordlist(const string &file)
{
	root = nullptr; // Initialize root to prevent undefined behavior
	ifstream inputFile(file);
	string word;
	this->size = 0;

	if (inputFile.is_open())
	{
		while (inputFile >> word)
		{
			this->insert(word);
		}
		inputFile.close();
	}
	else
	{
		cout << "Error: Unable to open file " << file << endl;
	}
}
bool Wordlist::remove(const string &word)
{
	if (!contains(word))
	{
		return false;
	}
	this->root = deleteNode(this->root, word);
	this->size--;
	return true;
}

int Wordlist::getCount(string word)
{
	return count(this->root, word);
}

int Wordlist::differentWords() const
{
	return this->size;
}

int Wordlist::totalWords() const
{
	return totalNumberOfWords(root, 0);
}

bool Wordlist::contains(string word)
{
	return getCount(word) > 0 ? true : false;
}

std::string intToString(int number) 
{
	if (number == 0)
	{
		return "0";
	}

	std::string result = "";

	while (number > 0)
	{
		int digit = number % 10;
		char ch = '0' + digit;
		result = ch + result;
		number /= 10;
	}

	return result;
}

string Wordlist::mostFrequent() const
{
	if (root == nullptr)
	{
		throw invalid_argument("Wordlist is empty");
	}
	else
	{
		AVLTreeNode *mostFre = mostFreq(root);
		string wordAndCount = mostFre->word + " " + intToString(mostFre->count);
		return wordAndCount;
	}
}
void Wordlist::inOrderTraversal(AVLTreeNode *root, int &counter) const
{
	if (root == nullptr)
	{
		return;
	}
	inOrderTraversal(root->left, counter);								 // Traverse left subtree
	cout << counter << ". " << root->word << " " << root->count << endl; // Visit node
	counter++;
	inOrderTraversal(root->right, counter); // Traverse right subtree
}

void Wordlist::printWords() const
{
	if (root == nullptr)
	{
		cout << "The tree is empty." << endl;
		return;
	}
	int counter = 1; // Start counter at 1 for printing
	inOrderTraversal(root, counter);
}

int Wordlist::singletons() const
{
	int count = 0;
	return singletonCount(root, count);
}

void Wordlist::printStatistics() const
{
	cout << "Number of different words: " << differentWords() << endl;
	cout << "    Total number of words: " << totalWords() << endl;
	cout << "       Most frequent word: " << mostFrequent() << endl;
	cout << "     Number of singletons: " << singletons()
		 << setprecision(0) << fixed
		 << " (" << 100.0 * singletons() / differentWords() << "%)"
		 << endl;
}
void Wordlist::insert(const string &word)
{
	
	if (!contains(word))
	{
		AVLTreeNode *newNode = new AVLTreeNode(word);
		newNode->count = 1;
		if (root == nullptr)
		{
			root = newNode;
		}
		else
		{
			AVLTreeNode *parent = insertionHelp(root, newNode);
			updateHeight(parent);
			balance(parent, word);
		}
		this->size++;
	}
	else
	{
		traverseIncrement(this->root, word);
	}
}

//**HELPERS**
AVLTreeNode *Wordlist::traversalCopy(AVLTreeNode *copy, AVLTreeNode *parent)
{
	if (copy == nullptr)
	{
		return nullptr;
	}
	else
	{
		AVLTreeNode *newNode = new AVLTreeNode(copy->word);
		newNode->parent = parent;
		newNode->count = copy->count;
		newNode->height = copy->height;
		newNode->left = traversalCopy(copy->left, newNode);
		newNode->right = traversalCopy(copy->right, newNode);
		return newNode;
	}
}

void Wordlist::postOrderTraversalDeletion(AVLTreeNode *n)
{
	if (n == nullptr)
	{
		return;
	}
	else
	{
		postOrderTraversalDeletion(n->left);
		postOrderTraversalDeletion(n->right);
		delete n;
	}
}

int Wordlist::maxInt(const int &a, const int &b)
{
	return a > b ? a : b;
}

int Wordlist::getHeight(AVLTreeNode *node)
{
	return (node == nullptr) ? 0 : node->height;
}

void Wordlist::updateHeight(AVLTreeNode *node)
{
	if (root != nullptr)
	{
		if (node->left == nullptr && node->right == nullptr)
		{
			node->height = 0;
		}
		else
		{
			node->height = 1 + maxInt(getHeight(node->left), getHeight(node->right));
		}
	}
}

int Wordlist::getBalanceFactor(AVLTreeNode *node)
{
	return getHeight(node->left) - getHeight(node->right);
}

void Wordlist::rotateLeft(AVLTreeNode *x)
{
	AVLTreeNode *y = x->right;
	AVLTreeNode *yLeftChild = y->left;

	y->left = x;
	x->right = yLeftChild;

	if (yLeftChild != nullptr)
	{
		yLeftChild->parent = x;
	}
	y->parent = x->parent;
	if (x->parent == nullptr)
	{
		root = y;
	}
	else if (x == x->parent->left)
	{
		x->parent->left = y;
	}
	else
	{
		x->parent->right = y;
	}
	x->parent = y;

	updateHeight(x);
	updateHeight(y);
}

void Wordlist::rotateRight(AVLTreeNode *y)
{
	AVLTreeNode *x = y->left;
	AVLTreeNode *xRightChild = x->right;

	x->right = y;
	y->left = xRightChild;

	if (xRightChild != nullptr)
	{
		xRightChild->parent = y;
	}
	x->parent = y->parent;
	if (y->parent == nullptr)
	{
		root = x;
	}
	else if (y == y->parent->left)
	{
		y->parent->left = x;
	}
	else
	{
		y->parent->right = x;
	}
	y->parent = x;
	updateHeight(y);
	updateHeight(x);
}

void Wordlist::balance(AVLTreeNode *node, string insertedWord)
{
	while (node != nullptr)
	{
		updateHeight(node);
		int balanceFactor = getBalanceFactor(node);
		if (balanceFactor > 1)
		{
			if (insertedWord < node->word)
			{
				rotateRight(node);
			}
			else
			{
				rotateLeft(node);
				rotateRight(node);
			}
		}
		else if (balanceFactor < -1)
		{
			if (insertedWord > node->word)
			{
				rotateLeft(node);
			}
			else
			{
				rotateRight(node);
				rotateLeft(node);
			}
		}
		node = node->parent;
	}
}

AVLTreeNode *Wordlist::insertionHelp(AVLTreeNode *root, AVLTreeNode *newNode)
{
	if (newNode->word < root->word)
	{
		if (root->left == nullptr)
		{
			root->left = newNode;
			newNode->parent = root;
			return root;
		}
		else
		{
			insertionHelp(root->left, newNode); // Recursively traverse the left subtree
		}
	}
	else if (newNode->word > root->word)
	{
		if (root->right == nullptr)
		{
			root->right = newNode;
			newNode->parent = root;
			return root;
		}
		else
		{
			insertionHelp(root->right, newNode); // Recursively traverse the right subtree
		}
	}
	return root;
}

void Wordlist::traverseIncrement(AVLTreeNode *root, const string &word)
{
	if (root == nullptr)
	{
		return;
	}
	else
	{
		if (word < root->word)
		{
			traverseIncrement(root->left, word);
		}
		else if (word > root->word)
		{
			traverseIncrement(root->right, word);
		}
		else
		{
			root->count++;
			return;
		}
	}
}

int Wordlist::count(AVLTreeNode *root, const string &word)
{
	if (root == nullptr)
	{
		return -1; // Word not found
	}
	if (word < root->word)
	{
		return count(root->left, word);
	}
	else if (word > root->word)
	{
		return count(root->right, word);
	}
	else
	{
		return root->count; // Word found
	}
}

int Wordlist::totalNumberOfWords(AVLTreeNode *root, int count) const
{
	if (root == nullptr)
	{
		return count;
	}
	else
	{
		count = totalNumberOfWords(root->left, count);
		count = totalNumberOfWords(root->right, count);
		return count + root->count;
	}
}

AVLTreeNode *Wordlist::max(AVLTreeNode *a, AVLTreeNode *b) const
{
	if (a == nullptr)
		return b;
	if (b == nullptr)
		return a;
	return (a->count > b->count) ? a : b;
}

AVLTreeNode *Wordlist::mostFreq(AVLTreeNode *root) const
{
	if (root == nullptr)
	{
		return nullptr;
	}

	AVLTreeNode *leftMax = mostFreq(root->left);
	AVLTreeNode *rightMax = mostFreq(root->right);

	return max(root, max(leftMax, rightMax));
}

int Wordlist::singletonCount(AVLTreeNode *root, int &count) const
{
	if (root == nullptr)
	{
		return count;
	}
	singletonCount(root->left, count);
	if (root->count == 1)
	{
		count++;
	}
	singletonCount(root->right, count);
	return count;
}

AVLTreeNode *Wordlist::inOrderPredecessor(AVLTreeNode *root)
{
	root = root->left;
	while (root->right != nullptr)
	{
		root = root->right;
	}
	return root;
}

AVLTreeNode *Wordlist::deleteNode(AVLTreeNode *root, string word)
{
	if (root == nullptr)
	{
		return nullptr;
	}
	else if (word < root->word)
	{
		root->left = deleteNode(root->left, word);
	}
	else if (word > root->word)
	{
		root->right = deleteNode(root->right, word);
	}
	else
	{
		// Case 1: No children
		if (root->left == nullptr & root->right == nullptr)
		{
			if (root->parent != nullptr)
			{
				if (root->parent->left == root)
				{
					root->parent->left == nullptr;
				}
				else
				{
					root->parent->right = nullptr;
				}
			}
			delete root;
			return nullptr;
		}
		else if (root->left == nullptr || root->right == nullptr)
		{
			AVLTreeNode *child = root->left == nullptr ? root->right : root->left;

			if (root->parent != nullptr)
			{
				if (root->parent->left == root)
				{
					root->parent->left = child;
				}
				else
				{
					root->parent->right = child;
				}
			}
			delete root;
			return child;
		}
		else
		{
			AVLTreeNode *iPre = inOrderPredecessor(root);
			root->left = deleteNode(root->left, iPre->word);

			iPre->left = root->left;
			iPre->right = root->right;

			if (root->parent == nullptr)
			{
				delete root;
				return iPre;
			}
			else
			{

				if (root->parent->left == root)
				{
					root->parent->left = iPre;
				}
				else
				{
					root->parent->right = iPre;
				}
			}
			iPre->parent = root->parent;
			delete root;
			return iPre;
		}
	}
	updateHeight(root);
	balance(root, word);
	return root;
}
