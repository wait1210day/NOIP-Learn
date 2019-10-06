#include <iostream>
#include <sstream>
#include <exception>
#include <stdexcept>
#include <algorithm>
#include <stack>
#include <string>
#include <map>
using namespace std;

#define AST_LEFT 	1
#define AST_RIGHT 	2
#define NODE_OPERATOR 	0
#define NODE_NUMBER		1

#define OPERATOR	0
#define NUMBER		1
#define UNKNOW		2
#define PRIFLAG1	3
#define PRIFLAG2	4

union ExpSymbols
{
	char arithmeticOperator;
	double arithmeticNumber;
};

struct ExpTreeNode
{
	double value;
	ExpSymbols data;
	ExpTreeNode *parent;
	ExpTreeNode *left;
	ExpTreeNode *right;
};

class exptree
{
public:
	explicit exptree();
	~exptree();
	
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
	void movIteratorToNewNode();
	
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
	 *				automatically. It will return NULL if there aren't
	 * 				any free nodes. It's always left-first, that means it will
	 * 				select left subtree to insert new node when left and right
	 * 				subtree are both free.
	 */
	ExpTreeNode *newNode(ExpSymbols data, ExpTreeNode *considerRoot, int rightOrLeft);
	ExpTreeNode *newNode(ExpSymbols data, int rightOrLeft);
	ExpTreeNode *newNode(ExpSymbols data);
	
	ExpSymbols nodeDataConstructor(double arithmeticNumber);
	ExpSymbols nodeDataConstructor(char arithmeticOperator);

	void travesalEachNode_FRO (
		void (*nodeOperation) (
			ExpTreeNode *selectedNode,
			exptree& classReference
		),
		ExpTreeNode *beginIterator
	);
	void travesalEachNode_LRO (
		void (*nodeOperation) (
			ExpTreeNode *selectedNode,
			exptree& classReference
		),
		ExpTreeNode *beginIterator
	);
	void travesalEachNode_MRO(
		void (*nodeOperation) (
			ExpTreeNode *selectedNode,
			exptree& classReference
		),
		ExpTreeNode *beginIterator
	);
	
	int getNodeType();
	int getNodeType(ExpTreeNode *node);
	
	/**
	 * Calculating each node's value, then we can get the value of
	 * all of the AST tree at root->value.
	 */
	double calculator();

private:
	static void destroyNode(ExpTreeNode *targetNode, exptree& ept);
	/* Calculating a node */
	static void nodeValueCalculator(ExpTreeNode *targetNode, exptree& ept);

private:
	// The root of abstruct syntax tree
	ExpTreeNode *ASTRoot;
	ExpTreeNode *iterator;
	ExpTreeNode *lastIterator;
	ExpTreeNode *lastNewNode;
};

exptree::exptree()
{
	this->ASTRoot = new ExpTreeNode;
	this->ASTRoot->left = nullptr;
	this->ASTRoot->right = nullptr;
	this->ASTRoot->parent = nullptr;
	this->iterator = this->ASTRoot;
	this->lastIterator = nullptr;
	this->lastNewNode = nullptr;
}

exptree::~exptree()
{
	travesalEachNode_LRO(this->destroyNode, root());
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

void exptree::movIteratorToNewNode()
{
	if (this->lastNewNode != nullptr)
		this->iterator = this->lastNewNode;
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
	this->lastNewNode = addedNode;
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
		
	return nullptr;
}

ExpSymbols exptree::nodeDataConstructor(double arithmeticNumber)
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

void exptree::travesalEachNode_FRO(void (*nodeOperation)(ExpTreeNode *, exptree&), ExpTreeNode *beginIterator)
{
	if (beginIterator == nullptr)
		return ;
	ExpTreeNode *left = beginIterator->left;
	ExpTreeNode *right = beginIterator->right;
	
	nodeOperation(beginIterator, *this);
	travesalEachNode_FRO(nodeOperation, left);
	travesalEachNode_FRO(nodeOperation, right);
}

void exptree::travesalEachNode_LRO(void (*nodeOperation)(ExpTreeNode *, exptree&), ExpTreeNode *beginIterator)
{
	if (beginIterator == nullptr)
		return ;
	ExpTreeNode *left = beginIterator->left;
	ExpTreeNode *right = beginIterator->right;
	
	travesalEachNode_LRO(nodeOperation, left);
	travesalEachNode_LRO(nodeOperation, right);
	nodeOperation(beginIterator, *this);
}

void exptree::travesalEachNode_MRO(void (*nodeOperation)(ExpTreeNode *, exptree&), ExpTreeNode *beginIterator)
{
	if (beginIterator == nullptr)
		return ;
	ExpTreeNode *left = beginIterator->left;
	ExpTreeNode *right = beginIterator->right;
	
	travesalEachNode_MRO(nodeOperation, left);
	nodeOperation(beginIterator, *this);
	travesalEachNode_MRO(nodeOperation, right);
}

void exptree::destroyNode(ExpTreeNode *targetNode, exptree& ept)
{
	if (targetNode != nullptr)
	{
		delete targetNode;
	}
}

int exptree::getNodeType()
{
	return getNodeType(this->iterator);
}

int exptree::getNodeType(ExpTreeNode *node)
{
	char opr = node->data.arithmeticOperator;
	if (opr == '+' || opr == '-'
		|| opr == '*' || opr == '/')
		return NODE_OPERATOR;
	return NODE_NUMBER;
}

void exptree::nodeValueCalculator(ExpTreeNode *targetNode, exptree& ept)
{
	if (ept.getNodeType(targetNode) == NODE_NUMBER)
	{
		targetNode->value = targetNode->data.arithmeticNumber;	
	}
	else
	{
		switch (targetNode->data.arithmeticOperator)
		{
			case '+':
				targetNode->value = targetNode->left->value
					+ targetNode->right->value;
				break;
			case '-':
				targetNode->value = targetNode->left->value
					- targetNode->right->value;
				break;
			case '*':
				targetNode->value = targetNode->left->value
					* targetNode->right->value;
				break;
			case '/':
				targetNode->value = targetNode->left->value
					/ targetNode->right->value;
				break;
		}
	}
}

double exptree::calculator()
{
	travesalEachNode_LRO(nodeValueCalculator, ASTRoot);
	return ASTRoot->value;
}

void nodePrint(ExpTreeNode *t, exptree& ept)
{
	if (ept.getNodeType(t) == NODE_OPERATOR)
		cout << t->data.arithmeticOperator << " ";
	else
		cout << t->data.arithmeticNumber << " ";
}

int getTokenType(string& token)
{
	int numberCount = 0;
	
	if (token[0] == '+' || token[0] == '-'
		|| token[0] == '*' || token[0] == '/')
		return OPERATOR;
	
	if (token[0] == '(')
		return PRIFLAG1;
	if (token[0] == ')')
		return PRIFLAG2;
	
	bool floating = false;
	for (char& byte : token)
	{
		if (byte >= '0' && byte <= '9')
			numberCount++;
		if (byte == '.' && !floating)
		{
			floating = true;
			numberCount++;
		}
	}
	if (numberCount == token.length())
		return NUMBER;
		
	return UNKNOW;
}

void prefixExpressionASTConstructor(exptree& ept, stack<string>& prefixExpr)
{
	string token;
	double num;
	bool rootWrote = false;
	
	while (!prefixExpr.empty())
	{
		token = prefixExpr.top();
		switch (getTokenType(token))
		{
			case NUMBER:
				num = atof(token.data());
				cout << "Prefix Parser: Insert number: " << num << endl;
				while (ept.newNode(ept.nodeDataConstructor(num)) == nullptr)
					ept.movIteratorRoot();
				break;
				
			case OPERATOR:
				cout << "Prefix Parser: Insert operator: " << token << endl;
			
				if (ept.thisNode() == ept.root() && !rootWrote)
				{
					ept.thisNode()->data.arithmeticOperator = token[0];
					rootWrote = true;
					break;
				}
	
				while (ept.newNode(ept.nodeDataConstructor(token[0])) == nullptr)
					ept.movIteratorRoot();
						
				ept.movIteratorToNewNode();
				break;
				
			case UNKNOW:
				cout << "Prefix Parser: Invalid input: " << token << endl;
				break;
		}
		prefixExpr.pop();
	}
}

void infixToPrefix(stack<string>& infix, stack<string>& prefix)
{
	string token;
	char markTop[2];
	markTop[1] = '\0';
	
	stack<char> mark;

    map<char, int> priority;
    priority['+'] = 0;
    priority['-'] = 0;
    priority['*'] = 1;
    priority['/'] = 1;
    
    while (!infix.empty())
	{
		token = infix.top();
		
        switch (getTokenType(token))
		{
            case NUMBER:
                prefix.push(token);
                break;
                
            case OPERATOR:
                if (!mark.empty())
				{
                    *markTop = mark.top();

                    while(*markTop != ')' && priority[token[0]] < priority[*markTop])
					{
                        prefix.push(markTop);
                        
                        mark.pop();
                        
                        if (mark.empty())
                            break;
                        *markTop = mark.top();
                    }
                }
                mark.push(token[0]);
                break;
                
            case PRIFLAG2:
                mark.push(')');
                break;
                
            case PRIFLAG1:
            	{
                    *markTop = mark.top();
	                
	                while(*markTop != ')')
					{
                        prefix.push(markTop);
	                    
	                    mark.pop();
	                    *markTop = mark.top();
	                }
	                mark.pop();
            	}
                break;
            default:
            	throw new runtime_error("Infix Parser: Invalid input \'" + token + "\'");
            	break;
        }
        
        infix.pop();
    }

    while(!mark.empty())
	{
		*markTop = mark.top();
        prefix.push(markTop);
        mark.pop();
    }
}

int main()
{
	stack<string> prefix, infix;
	string buf;
	exptree AST;
	
	while (cin >> buf)
		infix.push(buf);
	
	try
	{
		infixToPrefix(infix, prefix);
		prefixExpressionASTConstructor(AST, prefix);

		cout << "AST Constructor (MRO_NODEDUMPS): ";
		AST.travesalEachNode_MRO(nodePrint, AST.root());
		cout << endl << "AST Calculation (LRO_NODETRVAL): " << AST.calculator() << endl;
	}
	catch (runtime_error *except)
	{
		cerr << "Program aborted after throwing an RuntimeException: \n\t";
		cerr << except->what() << endl;
		delete except;
	}
	
	return 0;
}
