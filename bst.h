/******************************************************************************
Name: bst.h
Des: This file defines the Tree abstract data type. The tree is dynamically - 
allocated  binary search tree. The application programmer can insert into the 
tree, search fora criminal attribute and trim the criminals who do not match 
this attribute, and output the tree as an alphabetical list of criminals.
Author: Matthew Thompson
Date: 4/25/2017
******************************************************************************/
#pragma once
#include<string>
#include<fstream>
using namespace std;

struct Node;
typedef string TreeType;

// The node struct is the building block of a tree. Each node will store the 
// information about a criminal in the database. 
struct Node
{
	TreeType name;
	TreeType attributes[8];
	int attributeCount;
	Node * left;
	Node * right;
	Node * parent;
};

// The tree implementation includes many private functions
// which do the background work of inserting, deleting, and 
// traversing tree. The tree's root node is private so that
// the application cannot change the root by accident. 
class Tree
{
	public:
		Tree();
		~Tree();
		bool insert(TreeType, TreeType [8], int);
		void searchAndPrune(TreeType);
		void output(ofstream&);
		Node * find(string);
		bool insert(string);

	private:
		void postorderTraversalAndPrune(Node*, TreeType);
		bool deleteNode(Node*);
		void deleteLeafNode(Node*);
		void deleteOneChildNode(Node*);
		void deleteTwoChildNode(Node*);
		void inOrderTraversalAndOutput(Node*, ofstream&);
		void sort(Node*, Node*);
		Node * root;
};


