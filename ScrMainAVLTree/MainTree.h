#pragma once
#include<iostream>
#include <string>

struct DataOrder;
struct Date;
class List;
class HashTable;

std::ostream& operator << (std::ostream& out, DataOrder* data);
bool operator ==(DataOrder& data1, DataOrder& data2);
int SummDate(Date& date);
int ConvMonToInt(std:: string& m);

class NodeMainTree
{

public:
	unsigned int hight;
	NodeMainTree* left;
	NodeMainTree* right;
	unsigned int key;
	List* IndexList;
	NodeMainTree();
	~NodeMainTree();
};

class MainTree
{
private:
	HashTable* CourierHashTable;
	bool AddElem(DataOrder& data);
	NodeMainTree* AddInTreeElem(NodeMainTree* p, DataOrder& data);
	bool DelElem(DataOrder& data);
	NodeMainTree* DelInTreeElem(NodeMainTree* p, DataOrder& data);
	int FindElemInList(NodeMainTree* findData, DataOrder& data);
	NodeMainTree* RightTurn(NodeMainTree* node);
	NodeMainTree* LeftTurn(NodeMainTree* node);
	NodeMainTree* Rebalanse(NodeMainTree* node);
	void DeleteTree(NodeMainTree* node);
	NodeMainTree* maxLeft(NodeMainTree* p);
	NodeMainTree* RemoveMax(NodeMainTree* p);
	void FixedHight(NodeMainTree* p);
	int GetHight(NodeMainTree* p);
	int DifferHights(NodeMainTree* p);
	void ResizeArr();
	


public:
	int OrderArrSize;
	int NumElem;
	size_t Capacity;
	DataOrder** OrderArr;
	MainTree(HashTable* courierHashTable);
	~MainTree();
	NodeMainTree* root;
	std::pair<NodeMainTree*, int> SearchInTree(const unsigned int& passNum);
	bool AddElemInArr(DataOrder& elem);
	void FillArr(std::istream& in);

	bool DelElemInArr(DataOrder& elem);

	void PrintTree(NodeMainTree* root, int space = 0, int indent = 10, std::ostream& out = std::cout);
	void PrintOrderArr(std::ostream& out);

	void LogerTree(std::string text);
	void LogerTree(std::string text, int num);


};