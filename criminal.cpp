/******************************************************************************
Name: criminal.cpp
Des: This file builds a criminal database from criminal.txt and uses 
casefile.txt to process and prune the tree. The suspects that match the case 
file are output in alphabetical in criminal.out file. The criminal database is
constructed using an alphabetically sorted binary search tree, and criminals 
not matching the casefile are pruned from the tree.
Author: Matthew Thompson
Date: 4/25/2017
******************************************************************************/
#include"bst.h"
#include"vld.h"
#include<fstream>
#include<iostream>

// Function Prototypes
void constructDatabase(Tree & crimimalDatabase);

void main()
{
	ifstream fin("casefile.txt");
	ofstream fout("criminal.out");
	string inputLine;
	int caseNumber = 1;


	fin >> inputLine;
	// Read each inputline from casefile.txt until reaching "FINISHED" or the end of the file.
	while (inputLine != "FINISHED" && !fin.eof())
	{
		Tree criminalDatabase;
		constructDatabase(criminalDatabase);

		// Process each group of lines starting with the word "CASE".
		if (inputLine == "CASE")
		{
			// Output directly to criminal.out.
			fout << "CASE NUMBER " << caseNumber << endl;
			fout << "PRIME SUSPECTS ARE..." << endl;

			// Reads "TIP" before reading the actual tip content.
			fin >> inputLine;
			while (inputLine != "CASE" && inputLine != "FINISHED")
			{
				if (inputLine == "TIP")
				{
					fin >> inputLine;
				}
				// Trim the database down to only the criminals who match the tip.
				criminalDatabase.searchAndPrune(inputLine);
				fin >> inputLine;
			}
			// Output the criminals in alphabetical order.
			criminalDatabase.output(fout);
			fout << endl;
			caseNumber++;
		}
	}
	fin.close();


	Tree criminalDatabase;
	constructDatabase(criminalDatabase);
	Node * foundNode = criminalDatabase.find("ERIN-TAFT");
	if (foundNode != nullptr)
	{
		cout << foundNode->name << endl;
	}
	else
	{
		cout << "Node not found." << endl;
	}

	criminalDatabase.insert("MATTHEW-THOMPSON");
	criminalDatabase.insert("RAYNA-DEYOUNG");
	criminalDatabase.insert("LEW-KNAPP");
	criminalDatabase.insert("BETHANY-SIMMONS");
	criminalDatabase.insert("EMMA-ESSLINGER");
	criminalDatabase.insert("EMILY-MORRIS");
	criminalDatabase.insert("EMILY-NICKS");
	criminalDatabase.insert("EMILY-WIDEMAN");
	criminalDatabase.insert("EMILY-GREEN");
	criminalDatabase.insert("JACOB-MASON");
	criminalDatabase.insert("COLBERT-LEHR");
	criminalDatabase.insert("OLIVIA-LANUM");
	criminalDatabase.insert("ZEPH-LONG");
	criminalDatabase.output(fout);
	fout.close();
}

/******************************************************************************
Name: constructDatabase
Des: This function reads input from criminal.txt and constructs an alphabetical
binary search tree of criminals. Each node of the tree contains a name, up to 
eight criminal attributes, and an attribute count. 
******************************************************************************/
void constructDatabase(Tree & criminalDatabase)
{
	ifstream fin("criminal.txt");
	string inputLine;

	fin >> inputLine;
	while (!fin.eof())
	{
		// Read the group of lines starting with "SUSPECT" which give name
		// and multiple criminal attributes.
		if (inputLine == "SUSPECT")
		{
			string name;
			string attributes[8];
			int attributeCount = 0;

			fin >> inputLine;
			// Ensure that the suspect block actually has content. 
			if (inputLine != "SUSPECT" && inputLine != "FINISHED")
			{
				name = inputLine;
				fin >> inputLine;
				// Read multiple attributes listed below name. 
				while (inputLine != "SUSPECT" && inputLine != "FINISHED")
				{
					attributes[attributeCount] = inputLine;
					attributeCount++;
					fin >> inputLine;
				}
				// This dynamically creates a new node in the tree which recieves 
				// the processed information.
				criminalDatabase.insert(name, attributes, attributeCount);
			}
		}
	}
	fin.close();
}