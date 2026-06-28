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
	size_t MaxSizeArr;
	int NumElem;
	Cell** Table;
	DataCourier** CourierArr;
	int HashFunction(unsigned int& key);
	int HashFunction2(unsigned int& n, int& j);
	std::pair<int, int> ResolveCollisions(unsigned int& key, bool findForInsert = false);

public:
	size_t GetMaxSizeArr();
	int GetNumElem();
	Cell* GetCellTable(int& i);
	DataCourier* GetCellCourierArr(int& i);
	HashTable (size_t& Size);
	~HashTable();
	void SetOrderTree(MainTree*);
	std::pair <bool, int> AddInTable(DataCourier& data);
	bool DelInTable(DataCourier& data);
	std::pair <Cell*, int> SearchInHashTable(unsigned int& key);
};