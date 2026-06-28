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

Cell* HashTable::GetCellTable(int& i)
{
	return Table[i];
}
DataCourier* HashTable::GetCellCourierArr(int& i)
{
	return CourierArr[i];
}

size_t HashTable::GetMaxSizeArr()
{
	return this->MaxSizeArr;
}
int HashTable::GetNumElem()
{
	return this->NumElem;
}

int HashTable::HashFunction(unsigned int& key)
{
	size_t mask = this->MaxSizeArr - 1;
	return (key & mask); // то же что и (n + step) % this->MaxSizeArr;
}

int HashTable::HashFunction2(unsigned int& n, int& j)
{
	size_t mask = this->MaxSizeArr - 1;
	size_t step = (j * j + j) >> 1; // то же что и  (j*j + j)/2 = (0.5*j*j + 0.5*j)
	return ((n + step)  &  mask); // то же что и (n + step) % this->MaxSizeArr;
}



std::pair<int, int> HashTable::ResolveCollisions(unsigned int& key, bool findForInsert)
{
	int ind = HashFunction(key); // Стартовый хэш
	int NewInd = ind;
	int j = 1;
	int SearchSteps = 1; 
	int firstDeletedIndex = NO_CELL;


	while (j < this->MaxSizeArr && this->Table[NewInd]->state != EMPTY)
	{
		if (this->Table[NewInd]->state == OCCUPIED)
		{
			// Если ключ совпал, мы нашли элемент (для поиска/удаления) 
			// либо обнаружили дубликат (при добавлении)
			if (this->Table[NewInd]->key == key)
			{
				return { NewInd, SearchSteps };
			}
		}
		else if (this->Table[NewInd]->state == DELETED)
		{
	
			if (findForInsert && firstDeletedIndex == NO_CELL)
			{
				firstDeletedIndex = NewInd;
			}
		}

		NewInd = HashFunction2(key, j);
		j += 1;
		SearchSteps += 1;
	}

	// Если мы искали место для вставки и по пути встретили DELETED, возвращаем её индекс
	if (findForInsert && firstDeletedIndex != NO_CELL)
	{
		return { firstDeletedIndex, SearchSteps };
	}

	// Если вышли на EMPTY — возвращаем её индекс (для вставки). 
	// Если таблица полностью заполнена (j >= MaxSizeArr) и EMPTY не найден — возвращаем NO_CELL.
	int finalIndex = (this->Table[NewInd]->state == EMPTY) ? NewInd : NO_CELL;
	return { finalIndex, SearchSteps };
}

void HashTable::SetOrderTree(MainTree* orderTree)
{
	this->OrderTree = orderTree;
}
HashTable::HashTable(size_t& Size)
{
	
	this->NumElem = 0;
	this->MaxSizeArr = Size;
	this->Table = new Cell* [Size];
	for (size_t i = 0; i < Size; i++)
	{
		this->Table[i] = new Cell;
		this->Table[i]->state = EMPTY;
	}

	this->CourierArr = new  DataCourier * [Size];
	for (int i = 0; i < Size; i++) {

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



std:: pair <bool,int> HashTable::AddInTable(DataCourier& data)
{
	
	auto [index, steps] = ResolveCollisions(data.passNum, true);

	if (index == NO_CELL) return { false, NO_CELL }; //коэффициент заполненности слишком высок

	// Если ячейка уже занята таким же ключом (попытка вставить дубликат)
	if (this->Table[index]->state == OCCUPIED && this->Table[index]->key == data.passNum)
	{
		return { false, index };
	}

	// Записываем данные в найденную позицию (EMPTY или DELETED)
	this->Table[index]->key = data.passNum;
	this->Table[index]->index = NumElem;
	this->Table[index]->state = OCCUPIED;
	this->NumElem += 1;
	//Записываем данные в массив
	CourierArr[NumElem - 1] = new DataCourier;
	CourierArr[NumElem - 1]->fio = data.fio;
	CourierArr[NumElem - 1]->passNum = data.passNum;
	CourierArr[NumElem - 1]->transp = data.transp;
	return { true, index };
}




std::pair<Cell*, int> HashTable::SearchInHashTable(unsigned int& key)
{
	
	auto [index, steps] = ResolveCollisions(key, false);

	if (index != NO_CELL && this->Table[index]->state == OCCUPIED)
	{
		return { this->Table[index], steps }; // Успешный поиск: возвращаем ячейку и шаги
	}

	return { nullptr, steps }; // Неуспешный поиск: возвращаем nullptr и сколько шагов проверили
}

bool HashTable::DelInTable(DataCourier& data)
{
	auto [index, steps] = ResolveCollisions(data.passNum, false);

	// Проверяем, что элемент найден, он занят и данные полностью совпадают
	if (index == NO_CELL || this->Table[index]->state != OCCUPIED)
	{
		return false;
	}

	if (!(*CourierArr[this->Table[index]->index] == data))
	{
		return false; // Ключ совпал, но сами данные курьера другие
	}

	
	Cell* DelElem = this->Table[index];
	int LastElemInd = NumElem - 1;

	//Изменяем статус ячейки на удалённый
	DelElem->state = DELETED;

	//Удаляем из массива и переопределяем индекс последнего элемента
	delete CourierArr[DelElem->index];
	if (DelElem->index != LastElemInd)
	{
		CourierArr[DelElem->index] = CourierArr[LastElemInd];

		auto [updatedIndex, searchSteps] = ResolveCollisions(CourierArr[LastElemInd]->passNum, false);
		if (updatedIndex != NO_CELL)
		{
			this->Table[updatedIndex]->index = DelElem->index;
		}
	}


	CourierArr[LastElemInd] = nullptr;
	NumElem -= 1;

	return true;
}


