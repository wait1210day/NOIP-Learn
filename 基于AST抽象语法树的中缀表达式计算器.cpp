#include <iostream>
#include <exception>
#include <stdexcept>
using namespace std;

#define AST_LEFT 	1
#define AST_RIGHT 	2

union ExpSymbols
{
	char arithmeticOperator;
	int arithmeticNumber;
};

struct ExpTreeNode
{
	ExpSymbols data;
	ExpTreeNode *parent;
	ExpTreeNode *left;
	ExpTreeNode *right;
};

class exptree
{
public:
	explicit exptree();
	// ~exptree();
	
	/**
	 * All of our operations are based on 'iterator',
	 * movIterator* methods can move iterator to right,
	 * left, root, back and absolove root. By using
	 * thisNode() function, you can get a pointer of
	 * iterator.
	 */
	bool movIteratorLeft();
	bool movIteratorRight();
	bool movIteratorRoot();
	bool movIteratorBack();
	void movIteratorAbsroot();
	
	ExpTreeNode *thisNode();
	ExpTreeNode *root();
	
	/**
	 * Method newNode() can create a new AST node and
	 * insert it to AST tree in somewhere.
	 * Reference with 3 arguments:
	 * 				Create a new node, then using considerRoot as root,
	 *				insert a node that we created to left or right.
	 *
	 * Reference with 2 arguments:
	 *				It's similar to Reference 1, but it uses iterator as root.
	 *
	 * Reference with 1 argument:
	 *				It's similar to Reference 2, but it can select right/left
	 *				automatically. It will throw a runtime_error if there aren't
	 * 				any free nodes. It's always left-first, that means it will
	 * 				select left subtree to insert new node when left and right
	 * 				subtree are both free.
	 */
	ExpTreeNode *newNode(ExpSymbols data, ExpTreeNode *considerRoot, int rightOrLeft);
	ExpTreeNode *newNode(ExpSymbols data, int rightOrLeft);
	ExpTreeNode *newNode(ExpSymbols data);
	
	ExpSymbols nodeDataConstructor(int arithmeticNumber);
	ExpSymbols nodeDataConstructor(char arithmeticOperator);

private:
	void travesalEachNode(void (*nodeOperation)(void *), ExpTreeNode *beginIterator);
	void destroyNode(void *targetNode);

private:
	// The root of abstruct syntax tree
	ExpTreeNode *ASTRoot;
	ExpTreeNode *iterator;
	ExpTreeNode *lastIterator;
};

exptree::exptree()
{
	this->ASTRoot = new ExpTreeNode;
	this->ASTRoot->left = nullptr;
	this->ASTRoot->right = nullptr;
	this->ASTRoot->parent = nullptr;
	this->iterator = this->ASTRoot;
	this->lastIterator = nullptr;
}

bool exptree::movIteratorLeft()
{
	if (this->iterator->left == nullptr)
		return false;
	this->lastIterator = this->iterator;
	this->iterator = this->iterator->left;
	return true;
}

bool exptree::movIteratorRight()
{
	if (this->iterator->right == nullptr)
		return false;
	this->lastIterator = this->iterator;
	this->iterator = this->iterator->right;
	return true;
}

bool exptree::movIteratorBack()
{
	if (this->lastIterator == nullptr)
		return false;
	this->iterator = this->lastIterator;
	return true;
}

bool exptree::movIteratorRoot()
{
	if (this->iterator->parent == nullptr)
		return false;
	this->lastIterator = this->iterator;
	this->iterator = this->iterator->parent;
	return true;
}

void exptree::movIteratorAbsroot()
{
	this->iterator = this->ASTRoot;
}

ExpTreeNode *exptree::thisNode()
{
	return this->iterator;
}

ExpTreeNode *exptree::root()
{
	return this->ASTRoot;
}

ExpTreeNode *exptree::newNode(ExpSymbols data, ExpTreeNode *considerRoot, int rightOrLeft)
{
	if (considerRoot == nullptr)
		throw new runtime_error("Illegal arguemnt: Null pointer exception");
	
	ExpTreeNode *addedNode = new ExpTreeNode;
	addedNode->data = data;
	addedNode->left = nullptr;
	addedNode->right = nullptr;
	
	if ((considerRoot->left != nullptr && rightOrLeft == AST_LEFT)
		|| (considerRoot->right != nullptr && rightOrLeft == AST_RIGHT))
		throw new runtime_error("Illegal operation: insert: Override node");
	
	if (rightOrLeft == AST_LEFT)
		considerRoot->left = addedNode;
	else if (rightOrLeft == AST_RIGHT)
		considerRoot->right = addedNode;
	
	addedNode->parent = considerRoot;
	
	return addedNode;
}

ExpTreeNode *exptree::newNode(ExpSymbols data, int rightOrLeft)
{
	return newNode(data, thisNode(), rightOrLeft);
}

ExpTreeNode *exptree::newNode(ExpSymbols data)
{
	ExpTreeNode *pthis = thisNode();
	if (pthis->left == nullptr)
		return newNode(data, pthis, AST_LEFT);
	else if (pthis->right == nullptr)
		return newNode(data, pthis, AST_RIGHT);
	else
		throw new runtime_error("Unexpected operation: nodeSelection: Override node");
		
	return nullptr;
}

ExpSymbols exptree::nodeDataConstructor(int arithmeticNumber)
{
	ExpSymbols _data;
	_data.arithmeticNumber = arithmeticNumber;
	return _data;
}

ExpSymbols exptree::nodeDataConstructor(char arithmeticOperator)
{
	ExpSymbols _data;
	_data.arithmeticOperator = arithmeticOperator;
	return _data;
}

void FL_recursiveOutput(ExpTreeNode *begin)
{
	if (begin == nullptr)
		return ;
	
	FL_recursiveOutput(begin->left);
	cout << begin->data.arithmeticNumber << " ";
	FL_recursiveOutput(begin->right);
}
int main()
{
	exptree ept;
	
	try
	{
		ept.root()->data = ept.nodeDataConstructor(0);
	
		ept.newNode(ept.nodeDataConstructor(1), AST_LEFT);		
		ept.newNode(ept.nodeDataConstructor(2), AST_RIGHT);
		
		ept.movIteratorLeft();
		
		ept.newNode(ept.nodeDataConstructor(3), AST_LEFT);
		ept.newNode(ept.nodeDataConstructor(4), AST_RIGHT);
		
		ept.movIteratorRight();
		
		ept.newNode(ept.nodeDataConstructor(5), AST_LEFT);
		
		ept.movIteratorAbsroot();
		ept.movIteratorRight();
		
		ept.newNode(ept.nodeDataConstructor(6), AST_RIGHT);
	}
	catch (runtime_error *ecpt)
	{
		cout << "Operation aborted after throwing 'runtime_error':" << endl;
		cout << "    " << ecpt->what() << endl;
		delete ecpt;
	}
	
	FL_recursiveOutput(ept.root());
	
	return 0;
}
