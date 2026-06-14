#include"../ScrMainAVLTree/MainTree.h"
#include"../Stracts/Stracts.h"
#include"../ChekersInputData/CheckerInputData.h"
#include"HashTable.h"
#include<sstream>
#include<string>
#include<fstream>
#include<set>
using namespace std;

bool operator == (DataCourier data1, DataCourier data2){
	return ((data1.fio.f == data2.fio.f) &&
			(data1.fio.i == data2.fio.i) &&
			(data1.fio.o == data2.fio.o) &&
			(data1.transp.brand == data2.transp.brand) &&
			(data1.transp.model == data2.transp.model));
}

ostream& operator << (ostream& out, DataCourier* data)
{
	std::string NulDay;

	

	return out << data->passNum << " " << data->fio.f << " " << data->fio.i << " "
		<< NulDay << data->fio.o << " " << data->transp.brand << " " << data->transp.model
		<< endl;

};

bool HashTable::CheckUnikKey(unsigned int& key1, unsigned int& key2)
{
	return !(key1 == key2);
}

bool HashTable::isPrimeOptimized(size_t n) {
	if (n <= 1)
		return false;
	if (n == 2)
		return true;
	if (n % 2 == 0)
		return false;
	for (int i = 3; i <= sqrt(n); i += 2) {
		if (n % i == 0)
			return false;
	}
	return true;
}



int HashTable::HashFunction(const unsigned int& key)
{
	return (key % MaxSizeArr);
}

int HashTable::HashFunction2(int& n, int& j)
{
	return ((n + (int)pow(j, 2)) % this->MaxSizeArr);
}

int HashTable::ResolveCollis(int OriginalInd, int j)
{
	OriginalInd = HashFunction2(OriginalInd, j);
	return OriginalInd;
}


void HashTable::SetOrderTree(MainTree* orderTree)
{
	this->OrderTree = orderTree;
}
HashTable::HashTable(size_t Size)
{
	size_t NewSize = Size * 2;
	while (!isPrimeOptimized(NewSize))
	{
		NewSize += 1;
	}
	this->NumElem = 0;
	this->SizeArr = 0;
	this->MaxSizeArr = NewSize;
	this->Table = new Cell* [NewSize];
	for (size_t i = 0; i < NewSize; i++)
	{
		this->Table[i] = new Cell;
		this->Table[i]->state = EMPTY;
	}

	this->CourierArr = new  DataCourier * [NewSize];
	for (int i = 0; i < NewSize; i++) {

		this->CourierArr[i] = nullptr;
	}
	
}

HashTable::~HashTable()
{
	for (size_t i = 0; i < MaxSizeArr; i++)
    {
        delete Table[i];          // удаляем Cell
        if (CourierArr[i]!= nullptr) delete CourierArr[i];     // удаляем DataCourier
    }

    delete[] Table;
    delete[] CourierArr;
}



bool HashTable::AddElemInArr(DataCourier& data)
{
	
		if (AddInTable(data))
		{
			CourierArr[SizeArr] = new DataCourier;
			CourierArr[SizeArr]->fio = data.fio;
			CourierArr[SizeArr]->passNum = data.passNum;
			CourierArr[SizeArr]->transp = data.transp;
			SizeArr += 1;
			return true;
		}
		else return false;
}

bool HashTable::AddInTable(DataCourier& data)
{

	int ind = HashFunction(data.passNum);
	int j = 1;
	int DelCell = NO_CELL;
	while (CheckUnikKey(data.passNum, this->Table[ind]->key) && (this->Table[ind]->state != EMPTY) && (j < this->MaxSizeArr))
	{
		ind = ResolveCollis(ind, j);
		j += 1;
		if ((this->Table[ind]->state == DELETED) && (DelCell == NO_CELL)) DelCell = ind;
	}
	if (this->Table[ind]->state == EMPTY) {
		this->Table[ind]->key = data.passNum;
		this->Table[ind]->index = SizeArr;
		this->Table[ind]->state = OCCUPIED;
		this->NumElem += 1;
		return true;
	}
	else if (DelCell != NO_CELL) {
		this->Table[DelCell]->key = data.passNum;
		this->Table[DelCell]->index = SizeArr;
		this->Table[DelCell]->state = OCCUPIED;
		this->NumElem += 1;
		return true;
	}
	else {
		return false;
	}
	
}



void HashTable::FillArr(std::ifstream& in)
{
	string line;

	while (std::getline(in, line))
	{
		if (!line.empty())
		{
			std::istringstream iss(line);

			pair <DataCourier, std::string> elem = CheckInputCourier(iss);

			if (elem.second.empty())
			{
				AddElemInArr(elem.first);

			}
			// иначе просто ничего не делаем — строка пропущена
		}
	}
}
std::pair <Cell*, int> HashTable::SearchInHashTable(const unsigned int& key)
{
	unsigned int SearchKey = key;

	int SearchSteps = 1;
	int ind = HashFunction(key);
	int j = 1;
	while (j < this->MaxSizeArr)
	{
		if (this->Table[ind]->state == OCCUPIED)
		{
			if ((this->Table[ind]->key == SearchKey))
			{
				return { this->Table[ind],  SearchSteps};
			}
		}

		ind = ResolveCollis(ind, j);
		SearchSteps += 1;
		j += 1;
	}

	cout << "Elements not founded"<< endl;
	return { nullptr,  SearchSteps };

}

bool HashTable::DelElemInArr(DataCourier& data)
{
	if (OrderTree->SearchInTree(data.passNum).first == nullptr)
	{
		if (DelInTable(data) != true)
		{
			return false;
		}

		return true;
	}
}

bool HashTable::DelInTable(DataCourier& data)
{
	int count = 0;
	int ind = HashFunction(data.passNum);
	int j = 1;
	Cell* DelElem = nullptr;

	while ((j < this->MaxSizeArr) && (DelElem == nullptr))
	{
		if (this->Table[ind]->state == OCCUPIED)
		{
			if ((this->Table[ind]->key == data.passNum) && (*CourierArr[this->Table[ind]->index] == data))
			{
				DelElem = this->Table[ind];
			}
		}

		ind = ResolveCollis(ind,j);
		count += 1;
		j += 1;
	}

	if (j >= this->MaxSizeArr)
	{
		cout << "Elements not founded" << endl;
		return false;
		
	}
	else if (DelElem!= nullptr)
	{
		DelElem->state = DELETED;
		delete CourierArr[DelElem->index];
		CourierArr[DelElem->index] = nullptr;
		NumElem -= 1;
		cout << "Elements deleted" << endl;
	}
	return true;
	
}

void HashTable::PrintTable(std::ostream& out)
{
	for (size_t i = 0; i < this->MaxSizeArr; i++)
	{
		if (this->Table[i]->state == OCCUPIED)
		{
			out << i << ": " << "reserved -> ";
			out << CourierArr[ this->Table[i]->index];
		}
		else if(this->Table[i]->state == EMPTY) out << i << ": " << "Empty " << endl;
		else if (this->Table[i]->state == DELETED) out << i << ": " << "Delited " << endl;
	}
}

void HashTable::PrintCourierArr(std::ostream& out)
{
	for (int i = 0; i < SizeArr; i++)
	{
		if (CourierArr[i] != nullptr)
			out << CourierArr[i];
		else
			out << "nullptr" << endl;
			
	}
}
