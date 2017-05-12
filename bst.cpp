/******************************************************************************
Name: bst.cpp
Des: This file contains the implementation for the binary search tree abstract
data type.
Author: Matthew Thompson
Date: 4/25/2017
******************************************************************************/
#include"bst.h"
#include<iostream>
#include<fstream>

/******************************************************************************
Name: Tree (Constructor)
Des: This function constructs a binary search tree ADT. It simply sets the
root of the tree to nullptr to indicate an empty tree. 
******************************************************************************/
Tree::Tree()
{
	root = nullptr;
}

/******************************************************************************
Name: ~Tree (Destructor)
Des: This function destructs a dynamically allocated binary search tree by 
performing a postorder traversal. Once a node is visited, it is passed to the 
deleteNode function where it should be deleted properly. With a postorder 
traversal, each node visited should be a leaf node. This function will give 
back the dynamically allocated memory to the heap once the tree goes out of 
scope. 
******************************************************************************/
Tree::~Tree()
{
	Node * currentNode;
	while (root != nullptr)
	{
		currentNode = root;
		while (currentNode->left != nullptr)
		{
			currentNode = currentNode->left;
		}
		while (currentNode->right != nullptr)
		{
			currentNode = currentNode->right;
		}
		deleteNode(currentNode);
	}
}

/******************************************************************************
Name: insert
Des: This function attempts to create a new node and insert it into the tree.
If dynamic memory allocation fails, this function returns false. Otherwise, the
function returns true. The function uses the information passed from criminal.cpp
to fill the node, and it inserts the node at the proper location in the binary
search tree using the sort function.
******************************************************************************/
bool Tree::insert(TreeType name, TreeType attributes[8], int attributeCount)
{
	Node * newNode = new Node;
	// If dynamic memory allocation fails.
	if (newNode == nullptr)
	{
		return false;
	}
	else
	{
		// Add the information passed from criminal.cpp to the new node. 
		newNode->name = name;
		for (int i = 0; i < 8; i++)
		{
			newNode->attributes[i] = attributes[i];
		}
		newNode->attributeCount = attributeCount;
		newNode->left = nullptr;
		newNode->right = nullptr;
	}
	
	Node * currentNode = root;
	// If the tree is empty, the new node becomes the root node. 
	if (currentNode == nullptr)
	{
		root = newNode;
		newNode->parent = nullptr;
	}
	else
	{
		// Assign the node to the proper location in the binary search tree.
		sort(currentNode, newNode);
	}
	return true;
}

/******************************************************************************
Name: searchAndPrune
Des: This public function calls the private and recursive function 
postorderTraversalAndPrune. This function is needed to pass the root node to 
the function initially because the criminal.cpp file has no access to the 
private root of the tree. This function will cause any nodes not containing 
the attribute to be pruned from the tree.
******************************************************************************/
void Tree::searchAndPrune(TreeType attribute)
{
	Node * currentNode = root;
	postorderTraversalAndPrune(currentNode, attribute);
}

/******************************************************************************
Name: postorderTraversalAndPrune
Des: This recursive function performs a postorder traversal on the binary
search tree to find nodes which do not contain the attribute passed to the 
function. If the node lacks the requested attribute, the node is passed to 
deleteNode to be deleted. 
******************************************************************************/
void Tree::postorderTraversalAndPrune(Node * currentNode, TreeType attribute)
{
	// If the tree is not empty...
	if (currentNode != nullptr)
	{
		if (currentNode->left != nullptr)
		{
			postorderTraversalAndPrune(currentNode->left, attribute);
		}
		if (currentNode->right != nullptr)
		{
			postorderTraversalAndPrune(currentNode->right, attribute);
		}

		bool nodeHasAttribute = false;
		// Search for the requested attribute in the current node.
		for (int i = 0; i < currentNode->attributeCount; i++)
		{
			if (currentNode->attributes[i] == attribute)
			{
				nodeHasAttribute = true;
			}
		}
		if (!nodeHasAttribute)
		{
			deleteNode(currentNode);
		}
	}
}

/******************************************************************************
Name: deleteNode
Des: This function takes a node and decides which delete function to assign the
node to. The three cases (leaf, one-child, two-child) are each deleted 
differently, so proper distinction and function selection and crucial for the
success of the program. If the tree is empty, this function returns false.
******************************************************************************/
bool Tree::deleteNode(Node * currentNode)
{
	// If the tree is empty, return false. 
	if (root == nullptr)
	{
		return false;
	}
	else
	{
		// Leaf case
		if (currentNode->left == nullptr && currentNode->right == nullptr)
		{
			deleteLeafNode(currentNode);
		}
		// Two-child case
		else if (currentNode->left != nullptr && currentNode->right != nullptr)
		{
			deleteTwoChildNode(currentNode);
		}
		// One-child case
		else
		{
			deleteOneChildNode(currentNode);
		}
		return true;
	}
}

/******************************************************************************
Name: deleteLeafNode
Des: This function takes a leaf node and removes it from the tree. It sets 
the leaf's parent's pointer to the leaf to null and then deletes the leaf node. 
******************************************************************************/
void Tree::deleteLeafNode(Node * currentNode)
{
	// If the leaf node is the root:
	if (currentNode->parent == nullptr)
	{
		root = nullptr;
	}
	else
	{
		// If the leaf is a left child of its parent.
		if (currentNode->parent->left == currentNode)
		{
			currentNode->parent->left = nullptr;
		}
		// If the leaf is a right child of its parent.
		else
		{
			currentNode->parent->right = nullptr;
		}
	}
	delete currentNode;
}

/******************************************************************************
Name: deleteOneChildNode
Des: This function takes a node with only one child and prunes it from the tree.
It properly reassigns the parent left or right pointers pointing to the current
node to the node's one child and connects the one child's parent pointer to 
the current node's parent.
******************************************************************************/
void Tree::deleteOneChildNode(Node * currentNode)
{
	// If the current node is the root.
	if (currentNode->parent == nullptr)
	{
		if (currentNode->left != nullptr)
		{
			currentNode->left->parent = nullptr;
			root = currentNode->left;
		}
		else
		{
			currentNode->right->parent = nullptr;
			root = currentNode->right;
		}
	}
	else
	{
		// If the one child is the left child.
		if (currentNode->left != nullptr)
		{
			currentNode->left->parent = currentNode->parent;
			if (currentNode->left->name > currentNode->parent->name)
			{
				currentNode->parent->right = currentNode->left;
			}
			else
			{
				currentNode->parent->left = currentNode->left;
			}
		}
		// If the one child is the right child.
		else
		{
			currentNode->right->parent = currentNode->parent;
			if (currentNode->right->name > currentNode->parent->name)
			{
				currentNode->parent->right = currentNode->right;
			}
			else
			{
				currentNode->parent->left = currentNode->right;
			}
		}
	}
	delete currentNode;
}

/******************************************************************************
Name: deleteTwoChildNode
Des: This function takes a node with two children and properly prunes it from 
the tree. The algorithm finds the closest predecessor to the current node, 
replaces the current node's values with those of the closest predecessor's, and
then deletes the node that is the original closest predecessor.
******************************************************************************/
void Tree::deleteTwoChildNode(Node * currentNode)
{
	// Find closest predecessor node.
	Node * closestPredecessor = currentNode;
	closestPredecessor = closestPredecessor->left;
	while (closestPredecessor->right != nullptr)
	{
		closestPredecessor = closestPredecessor->right;
	}

	// Replace the data in the two child node with the data from the predecessor node.
	currentNode->name = closestPredecessor->name;
	for (int i = 0; i < 8; i++)
	{
		currentNode->attributes[i] = closestPredecessor->attributes[i];
	}
	currentNode->attributeCount = closestPredecessor->attributeCount;

	deleteNode(closestPredecessor);
}

/******************************************************************************
Name: output
Des: This function calls the recursive private function inOrderTraversalAndOutput
which performs and in-order traversal and outputs the nodes individually. This
function is required to pass the root into the function from a public function
that is in the tree class. Criminal.cpp has no access to the root of the tree.
******************************************************************************/
void Tree::output(ofstream& fout)
{
	Node * currentNode = root;
	inOrderTraversalAndOutput(currentNode, fout);
}

/******************************************************************************
Name: inOrderTraversalAndOutput
Des: This function performs an in-order traversal of the binary tree and 
outputs the node's name field on each process step. This outputs to the file
output stream at criminal.out.
******************************************************************************/
void Tree::inOrderTraversalAndOutput(Node * currentNode, ofstream& fout)
{
	// If the tree is empty, there were no criminals who had an attribute that
	// matched the request. 
	if (root == nullptr)
	{
		fout << "NO SUSPECTS FOR THIS CASE" << endl;
	}
	else if(currentNode != nullptr)
	{
		inOrderTraversalAndOutput(currentNode->left, fout);
		fout << currentNode->name << endl;
		inOrderTraversalAndOutput(currentNode->right, fout);
	}
}

/******************************************************************************
Name: sort
Des: This function uses the new node's name and properly sorts the new node
into the tree alphabetically. If the new node comes before the current node 
alphabetically, it proceeds to the current node's left child and reevaluates.
If there is no child to go to, the traversal stops and the node is inserted 
into the tree at this place. 
******************************************************************************/
void Tree::sort(Node * currentNode, Node * newNode)
{
	if (newNode->name > currentNode->name)
	{
		if (currentNode->right != nullptr)
		{
			sort(currentNode->right, newNode);
		}
		else
		{
			currentNode->right = newNode;
			newNode->parent = currentNode;
		}
	}
	else
	{
		if (currentNode->left != nullptr)
		{
			sort(currentNode->left, newNode);
		}
		else
		{
			currentNode->left = newNode;
			newNode->parent = currentNode;
		}
	}
}

Node * Tree::find(string key)
{
	if (root == nullptr)
	{
		return nullptr;
	}
	else
	{
		Node * currentNode = root;
		while (currentNode != nullptr)
		{
			if (key == currentNode->name)
			{
				return currentNode;
			}
			else
			{
				if (key < currentNode->name)
				{
					currentNode = currentNode->left;
				}
				else
				{
					currentNode = currentNode->right;
				}
			}
		}
		// If nothing was found
		return nullptr;
	}
}

bool Tree::insert(string key)
{
	Node * newNode = new Node;
	if (newNode == nullptr)
	{
		return false;
	}
	else
	{
		newNode->name = key;
		newNode->left = nullptr;
		newNode->right = nullptr;
		Node * currentNode = root;
		bool nodeInserted = false;
		while (nodeInserted != true)
		{
			if (root == nullptr)
			{
				root = newNode;
				newNode->parent = nullptr;
				nodeInserted = true;
			}
			else
			{
				if (key < currentNode->name)
				{
					if (currentNode->left == nullptr)
					{
						currentNode->left = newNode;
						newNode->parent = currentNode;
						nodeInserted = true;
					}
					else
					{
						currentNode = currentNode->left;
					}
				}
				else
				{
					if (currentNode->right == nullptr)
					{
						currentNode->right = newNode;
						newNode->parent = currentNode;
						nodeInserted = true;
					}
					else
					{
						currentNode = currentNode->right;
					}
				}
			}
		}
		return true;
	}
}