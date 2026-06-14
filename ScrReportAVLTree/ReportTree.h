#pragma once
#include<iostream>
#include"../Stracts/Stracts.h"
class MainTree;
class HashTable;
class NodeMainTree;
class List;
struct FIO;
struct DataReport;
struct Date;
struct Adres;
struct DataOrder;

bool operator ==(FIO& fio1, FIO& fio2);
bool operator ==(Adres& adres1, Adres& adres2);
std::ostream& operator << (std::ostream& out, DataReport* data);
class List;
class NodeReportTree
{

public:
	unsigned int hight;
	NodeReportTree* left;
	NodeReportTree* right;
	Date key;
	List* IndexList;
	NodeReportTree();
	~NodeReportTree();
};

class ReportTree
{
private:
	MainTree* mainTree;
	HashTable* mainHashTable;
	int FindElemInListMainTree(NodeMainTree* findData, DataOrder* data);
	NodeReportTree* AddInReportTreeElem(NodeReportTree* p, DataOrder* data);
	NodeReportTree* RightTurn(NodeReportTree* node);
	NodeReportTree* LeftTurn(NodeReportTree* node);
	NodeReportTree* Rebalanse(NodeReportTree* node);
	void DeleteReportTree(NodeReportTree* node);
	void FixedHight(NodeReportTree* p);
	int GetHight(NodeReportTree* p);
	int DifferHights(NodeReportTree* p);
	void ResizeReportArr();



public:
	int SizeReportArr;
	int NumElem;
	size_t CapacityReportArr;
	DataReport** ReportArr;

	ReportTree(MainTree* mainTree, HashTable* mainHashTable);
	~ReportTree();
	NodeReportTree* ReportTreeroot;
	NodeReportTree* SearchElemInReportTree(Date& date);
	bool GenerateReport(Date& date, Adres& adres, FIO& fio);
	void FillReoortTree();
	void PrintReportTree(NodeReportTree* root, int space = 0, int indent = 10, std::ostream& out = std::cout);
	void PrintReport(std::ostream& out);
	void LogerReport(std::string text);
	void LogerReport(std::string text, int num);


};