#pragma once
#include<iostream>
struct DataCourier;
struct FIO;
struct Cell;
class MainTree;

bool operator == (DataCourier data1, DataCourier data2);
std::ostream& operator << (std::ostream& out, DataCourier* data);
constexpr uint8_t EMPTY = 0;
constexpr uint8_t OCCUPIED = 1;
constexpr uint8_t DELETED = 2;
constexpr int NO_CELL = -1;


class HashTable
{
private:
	MainTree* OrderTree;
	bool AddInTable(DataCourier& data);
	bool DelInTable(DataCourier& data);
	bool CheckUnikKey(unsigned int& key1, unsigned int& key2);
	bool isPrimeOptimized(size_t n);
	int HashFunction(const unsigned int& key);
	int HashFunction2(int& n, int& j);
	int ResolveCollis(int OriginalInd, int j);
public:
	Cell** Table;
	DataCourier** CourierArr;
	size_t SizeArr;
	size_t MaxSizeArr;
	int NumElem;
	HashTable (size_t Size);
	~HashTable();
	void SetOrderTree(MainTree*);
	void FillArr(std::ifstream & in);
	bool AddElemInArr(DataCourier& data);
	bool DelElemInArr(DataCourier& data);
	std::pair <Cell*, int> SearchInHashTable(const unsigned int& key);
	void PrintTable(std::ostream& out);
	void PrintCourierArr(std::ostream& out);
	void LogerHashTable(std::string text);
	void LogerHashTable(std::string text, int num);
};