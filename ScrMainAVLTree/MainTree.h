#pragma once
#include<iostream>
#include <string>
#include"../Stracts/Stracts.h"

struct DataOrder;
struct Date;
class List;
class HashTable;
class ReportTree;
class MainTree;
class NodeMainTree;
struct FIO;
struct DataReport;
struct Adres;


bool operator ==(FIO& fio1, FIO& fio2);
bool operator ==(Adres& adres1, Adres& adres2);
std::ostream& operator << (std::ostream& out, DataReport* data);
std::ostream& operator << (std::ostream& out, DataOrder* data);
bool operator ==(DataOrder& data1, DataOrder& data2);
int SummDate(Date& date);



class BaseNode {
public:
	unsigned int hight;
	BaseNode* left;
	BaseNode* right;
	List* IndexList; 
	virtual void SetKey(const unsigned int& newKey) = 0;
	virtual unsigned int GetKey() = 0;
	BaseNode();
	virtual ~BaseNode();

};

class NodeMainTree: public BaseNode
{
private:
	unsigned int key;
public:
	
	unsigned int GetKey() override;
	void SetKey(const unsigned int& newKey)override;

	NodeMainTree(unsigned int& key);
};


class NodeReportTree : public BaseNode
{
private:
	Date key;
public:
	
	unsigned int GetKey() override;
	void SetKey(const unsigned int& newKey)override;
	NodeReportTree(Date& key);
};



class BaseAVLTree {
protected:

	
	BaseNode* maxLeft(BaseNode* p);
	BaseNode* RemoveMax(BaseNode* p);
	int GetHight(BaseNode* p);
	int DifferHights(BaseNode* p);
	void FixedHight(BaseNode* p);
	BaseNode* RightTurn(BaseNode* p);
	BaseNode* LeftTurn(BaseNode* q);
	BaseNode* Rebalanse(BaseNode* p);
	void DeleteTree(BaseNode* node);
	int FindElemInListNodeTree(BaseNode* findData, DataOrder& data);
public:
	BaseNode* root;
	virtual BaseNode* AddInTreeElem(BaseNode* p, DataOrder& data) = 0;
	virtual BaseNode* DelInTreeElem(BaseNode* p, DataOrder& data) = 0;
	virtual std::pair<BaseNode*, int> SearchInTree(const unsigned int& key) = 0;
	virtual DataOrder* GetCellOrderArr(int& i) = 0;
	

};


class MainTree : public BaseAVLTree
{
private:
	
	size_t CapacityOrdersArr;
	int NumElemOrdersArr;
	HashTable* CourierHashTable;
	BaseAVLTree* reportTree;
	DataOrder** OrderArr;
	BaseNode* AddInTreeElem(BaseNode* p, DataOrder& data) override;
	BaseNode* DelInTreeElem(BaseNode* p, DataOrder& data) override;
	void IncreaseArr();
	void DecreaseArr();
	

public:
	
	size_t GetCapasityOrdersArr();
	int GetNumElemOrdersArr();
	void SetReportTree(ReportTree* reportTree);
	DataOrder* GetCellOrderArr(int& i) override;
	MainTree(HashTable* courierHashTable);
	~MainTree();
	
	bool AddElemInArr(DataOrder& elem);
	bool DelElemInArr(DataOrder& elem);
	std::pair<BaseNode*, int> SearchInTree(const unsigned int& key) override;
	

};


class ReportTree : public BaseAVLTree
{
private:
	DataReport** ReportArr;
	size_t CapacityReportArr;
	int NumElemReportArr;
	BaseAVLTree* mainTree;
	HashTable* CourierHashTable;
	void ResizeReportArr();



public:

	
	
	size_t GetCapasityReportArr();
	int GetNumElemReportArr();
	DataOrder* GetCellOrderArr(int& i) override;

	ReportTree(MainTree* mainTree, HashTable* mainHashTable);
	~ReportTree();
	
	void ClearReportArr();
	bool GenerateReport(Date& date, Adres& adres, FIO& fio);
	void PrintReport(std::ostream& out);

	BaseNode* AddInTreeElem(BaseNode* p, DataOrder& data) override;
	BaseNode* DelInTreeElem(BaseNode* p, DataOrder& data) override;
	std::pair<BaseNode*, int> SearchInTree(const unsigned int& key) override;
	


};