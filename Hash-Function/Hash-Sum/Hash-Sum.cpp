#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <cstdlib>
#include <iomanip>
using namespace std;

const int sizeMassive = 100;

struct Node {
	string word;
	int count;
	Node* next;
};
typedef Node* PNode;


PNode CreateNode(string word)
{
	PNode pNewNode = new Node;
	pNewNode->word = word;
	pNewNode->count = 1;
	pNewNode->next = nullptr;
	return pNewNode;
}


void AddNodeInList(PNode* BeginList, string word, int hash)
{
	PNode pNewNode = CreateNode(word);
	PNode pElementList = BeginList[hash];
	while (pElementList->next)
	{
		pElementList = pElementList->next;
	}
	pElementList->next = pNewNode;
}


bool FindWordInList(PNode* BeginList, string word, int hash)
{
	PNode p = BeginList[hash];
	while (true)
	{
		if (!p)
			return false;
		if ((p->word) == word)
		{
			p->count++;
			return true;
		}
		p = p->next;
	}
}


int HashFunc(string& word)
{

	for (int i = 0; i < word.size(); i++)
	{
		word[i] = tolower(word[i]);
		if (word[i] >= 97 && word[i] <= 122 || word[i] == '-' || word[i] == '\'')
		{
			continue;
		}
		else if (i == 0 && (word[i] == '(' || word[i] == '"'))
		{
			word.erase(0, 1);
			i--;
		}
		else
		{
			word.erase(i, word.size() - i);
			break;
		}
	}
	if (!word.size())
	{
		return -1;
	}

	int hash = 0;
	for (int i = 0; i < word.size(); i++)
	{
		hash += word[i];
	}
	return hash % sizeMassive;
}

void DeleteAllTable(PNode* BeginList)
{
	for (int i = 0; i < sizeMassive; i++)
	{
		while (BeginList[i])
		{
			PNode willDeleted = BeginList[i];
			BeginList[i] = BeginList[i]->next;
			delete willDeleted;
		}
	}
}


int main()
{
	PNode BeginList[sizeMassive];
	for (int i = 0; i < sizeMassive; i++)
	{
		BeginList[i] = nullptr;
	}
	ifstream read("input.txt");
	if (!read.is_open())
		cout << "Error opening file input.txt\n";

	string word;
	while (!read.eof())
	{
		read >> word;
		int hash = HashFunc(word);
		if (hash == -1)
			continue;

		if (!FindWordInList(BeginList, word, hash))
		{
			AddNodeInList(BeginList, word, hash);
		}
	}

	read.close();
	ofstream foutput("output.txt");
	if (!foutput.is_open())
		cout << "Error opening file output.txt\n";
	ofstream fexceloutput("excel_output.txt");
	if (!fexceloutput.is_open())
		cout << "Error opening file excel_output.txt\n";

	PNode p;
	int uniqueWords = 0;
	for (int i = 0; i < sizeMassive; i++)
	{
		p = BeginList[i];
		int lengthList = 0;
		while (p)
		{
			foutput << p->word << ' ' << p->count << endl; //<< ' ' << i 
			lengthList++;
			p = p->next;
		}
		fexceloutput << i << ' ' << lengthList << endl;
		uniqueWords += lengthList;
	}
	foutput << endl << "Number of unique words = " << uniqueWords;
	foutput.close();
	fexceloutput.close();
	DeleteAllTable(BeginList);
	return 0;
}