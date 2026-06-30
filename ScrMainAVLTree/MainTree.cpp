#include "MainTree.h"
#include"../List/ListTree.h"
#include"../ScrHashTable/HashTable.h"
#include"../ChekersInputData/CheckerInputData.h"
#include <map>
#include<locale>
#include<sstream>
#include"../Stracts/Stracts.h"
#include <string>

using namespace std;
bool operator ==(DataOrder& data1, DataOrder& data2)
{
    return ((data1.adres.house == data2.adres.house) &&
            (data1.adres.street == data2.adres.street) &&
            (data1.date.d == data2.date.d) &&
            (data1.date.m == data2.date.m) &&
            (data1.date.y == data2.date.y) &&
            (data1.price == data2.price));
}
ostream& operator << (ostream& out, DataOrder* data)
{
    std::string NulDay;

    if (data->date.d < 10) NulDay = '0';
    else NulDay = "";

    return out << data->passNum << " " << data->adres.street << " " << data->adres.house << " "
        << NulDay << data->date.d << " " << data->date.m << " " << data->date.y
        << " " << data->price << endl;

};
bool operator ==(FIO& fio1, FIO& fio2)
{
    return ((fio1.f == fio2.f) && (fio1.i == fio2.i) && (fio1.o == fio2.o));
}

bool operator ==(Adres& adres1, Adres& adres2)
{
    return ((adres1.house == adres2.house) && (adres1.street == adres2.street));
}

ostream& operator << (ostream& out, DataReport* data)
{
    std::string NulDay;

    if (data->date.d < 10) NulDay = '0';
    else NulDay = "";

    return out
        << " " << data->passNum
        << " " << NulDay << data->date.d << " " << data->date.m << " " << data->date.y
        << " " << data->adres.street << " " << data->adres.house
        << " " << data->fio.f << " " << data->fio.i << " " << data->fio.o
        << " " << data->transp.brand << " " << data->transp.model
        << " " << data->price << endl;
};


int SummDate(Date& date) {
    return date.y * 10000 + ConvMonToInt(date.m) * 100 + date.d;
}


BaseNode::BaseNode() {
    hight = 1;
    left = nullptr;
    right = nullptr;
    IndexList = new List();
}
BaseNode::~BaseNode()
{
    delete IndexList;
}

unsigned int NodeMainTree::GetKey()
{
    return this->key;
}
void NodeMainTree::SetKey(const unsigned int& newKey)
{
    this->key = newKey;
}


void MainTree::IncreaseArr()
{
    CapacityOrdersArr *= 2;

    DataOrder** newArr = new DataOrder * [CapacityOrdersArr];

    for (int i = 0; i < NumElemOrdersArr; i++)
        newArr[i] = OrderArr[i];

    for (int i = NumElemOrdersArr; i < CapacityOrdersArr; i++)
        newArr[i] = nullptr;

    delete[] OrderArr;
    OrderArr = newArr;
}

void MainTree::DecreaseArr()
{
    if (CapacityOrdersArr <= 2) return;
    CapacityOrdersArr /= 2;

    DataOrder** newArr = new DataOrder * [CapacityOrdersArr];

    for (int i = 0; i < NumElemOrdersArr; i++)
        newArr[i] = OrderArr[i];

    delete[] OrderArr;
    OrderArr = newArr;
}

void MainTree::SetReportTree(ReportTree* reportTree)
{
    this->reportTree = reportTree;
}
size_t MainTree::GetCapasityOrdersArr()
{
    return this->CapacityOrdersArr;
}

int MainTree::GetNumElemOrdersArr()
{
    return this->NumElemOrdersArr;
}

DataOrder* MainTree::GetCellOrderArr(int& i)
{
    return  this->OrderArr[i];
}




int BaseAVLTree::GetHight(BaseNode* p)
{
    return p == nullptr ? 0 : p->hight;
}
int BaseAVLTree::DifferHights(BaseNode* p)
{
    return GetHight(p->right) - GetHight(p->left);
}
void BaseAVLTree::FixedHight(BaseNode* p)
{
    unsigned int hl = GetHight(p->left);
    unsigned int hr = GetHight(p->right);
    p->hight = (hl > hr ? hl : hr) + 1;
}


int BaseAVLTree::FindElemInListNodeTree(BaseNode* findData, DataOrder& data)
{
    NodeList* p = findData->IndexList->h;
    if (p->next == findData->IndexList->h)
    {
        if (*GetCellOrderArr(p->Index) == data) return p->Index;
    }
    else
    {
        while (p->next != findData->IndexList->h)
        {
            if (*GetCellOrderArr(p->Index) == data) return p->Index;
            p = p->next;
        }
        if (*GetCellOrderArr(p->Index) == data) return p->Index;
    }

    return -1;
}

NodeMainTree::NodeMainTree(unsigned int& key): key(key), BaseNode() {}


MainTree::MainTree(HashTable* courierHashTable) {

    this->CourierHashTable = courierHashTable;
    this->CourierHashTable->SetOrderTree(this);
    
    this->root = nullptr;
    this->CapacityOrdersArr = 2;
    this->NumElemOrdersArr = 0;
    this->OrderArr = new DataOrder*[CapacityOrdersArr];
    for (int i = 0; i < CapacityOrdersArr; i++) {
        
        this->OrderArr[i] = nullptr;
    }
}

void BaseAVLTree::DeleteTree(BaseNode* node) {
    if (node == nullptr) return;

    DeleteTree(node->left);
    DeleteTree(node->right);
    delete node;
}

MainTree::~MainTree() {
    
        for (int i = 0; i < NumElemOrdersArr; i++)
            if(OrderArr[i] != nullptr)
                delete OrderArr[i];
        delete[] OrderArr;
    DeleteTree(root);
    this->root = nullptr;
}

ReportTree::~ReportTree() {

    for (int i = 0; i < NumElemReportArr; i++)
        if (ReportArr[i] != nullptr)
        delete ReportArr[i];
    delete[] ReportArr;
    DeleteTree(root);
    this->root = nullptr;
}


BaseNode* BaseAVLTree::RightTurn(BaseNode* node) {
    BaseNode* q = node->left;
    node->left = q->right;
    q->right = node;
    if (node == root) root = q;
    FixedHight(node);
    FixedHight(q);
    return q;
}

BaseNode* BaseAVLTree::LeftTurn(BaseNode* node) {
    BaseNode* p = node->right;
    node->right = p->left;
    p->left = node;
    if (node == root) root = p;
    FixedHight(node);
    FixedHight(p);
    return p;
}
BaseNode* BaseAVLTree::Rebalanse(BaseNode* node) {
    BaseNode* p = node;
    if (DifferHights(p) == 2)
    {
        if (DifferHights(p->right) < 0)
        {
            p->right = RightTurn(p->right);
        }
        return LeftTurn(p);

    }
    else if (DifferHights(p) == -2)
    {
        if (DifferHights(p->left) > 0)
        {
            p->left = LeftTurn(p->left);
        }
        return RightTurn(p);
    }

    return p;
}


bool MainTree::AddElemInArr(DataOrder& elem)
{
    int SizeOld = this->NumElemOrdersArr;
    AddInTreeElem(this->root, elem);
    int SizeNew = this->NumElemOrdersArr;
    if (SizeOld!= SizeNew)
    {

        if (NumElemOrdersArr >= CapacityOrdersArr)
            IncreaseArr();

        OrderArr[NumElemOrdersArr - 1] = new DataOrder;
        OrderArr[NumElemOrdersArr - 1]->adres = elem.adres;
        OrderArr[NumElemOrdersArr - 1]->date = elem.date;
        OrderArr[NumElemOrdersArr - 1]->price = elem.price;
        OrderArr[NumElemOrdersArr - 1]->passNum = elem.passNum;
        reportTree->AddInTreeElem(reportTree->root, elem);
        return true;

    }
    else return false;    
}



BaseNode* MainTree::AddInTreeElem(BaseNode* p, DataOrder& data) {
   
    if (p == nullptr) { // добавление нового узла
        p = new NodeMainTree(data.passNum);
        p->IndexList->AddElemInList(NumElemOrdersArr);
        NumElemOrdersArr += 1;
        if (root == nullptr) root = p;
        return p;
    }
    unsigned int passNamData = data.passNum;
    unsigned int passNamDataP = p->GetKey();
    if (passNamData < passNamDataP){
        p->left = AddInTreeElem(p->left, data);
        FixedHight(p);
    }
    else if (passNamData > passNamDataP){
        p->right = AddInTreeElem(p->right, data);
        FixedHight(p);
    }
    else{        
            p->IndexList->AddElemInList(NumElemOrdersArr);
            NumElemOrdersArr += 1;
    }
    return Rebalanse(p);
}

std::pair<BaseNode*, int> MainTree::SearchInTree( const unsigned int & key) {

    int SearchSteps = 1;
    BaseNode* p = root;
    while (p != nullptr) {

        if (key < p->GetKey()){
            p = p->left;
            SearchSteps += 1;
        }
        else if (key > p->GetKey()) {
            p = p->right;
            SearchSteps += 1;
        }
        else return {p, SearchSteps };
    }
    return { nullptr, SearchSteps };
}


BaseNode* BaseAVLTree::maxLeft(BaseNode* p) {
    if (p->right == nullptr) return p;
    return maxLeft(p->right);

}
BaseNode* BaseAVLTree::RemoveMax(BaseNode* p)
{
    if (p->right == nullptr)
    {
        BaseNode* leftChild = p->left;
        delete p;
        return leftChild;
    }
    p->right = RemoveMax(p->right);
    FixedHight(p);
    return Rebalanse(p);
}

bool  MainTree::DelElemInArr(DataOrder& elem)
{
    int SizeOld = this->NumElemOrdersArr;
    DelInTreeElem(this->root, elem);
    int SizeNew = this->NumElemOrdersArr;
    if (SizeOld != SizeNew)
    {
        if (NumElemOrdersArr == (CapacityOrdersArr/2))
            DecreaseArr();

        return true;
        
    }
    return false;
}




BaseNode* MainTree::DelInTreeElem(BaseNode* p, DataOrder& data) {

    if (p == nullptr) {
        return nullptr;
    }
    unsigned int passNamData = data.passNum;
    unsigned int passNamDataP = p->GetKey();
    if (passNamData < passNamDataP)
    {
        p->left = DelInTreeElem(p->left, data);
        FixedHight(p);
    }
    else if (passNamData > passNamDataP)
    {
        p->right = DelInTreeElem(p->right, data);
        FixedHight(p);
    }
    else
    {
        int FindDelIndex = FindElemInListNodeTree(p, data);
        int LastElemInd = NumElemOrdersArr - 1;
        if (FindDelIndex != -1)
        {
            if (this->reportTree != nullptr) this->reportTree->DelInTreeElem(reportTree->root, data);

            p->IndexList->DelElemInList(FindDelIndex);
            delete OrderArr[FindDelIndex];
            // заполнение пустой ячейки списка заказов после удаления на последний элемент
            if (FindDelIndex != LastElemInd) 
            {
                OrderArr[FindDelIndex] = OrderArr[LastElemInd];
                List* ListWithMaxInd = SearchInTree(OrderArr[LastElemInd]->passNum).first->IndexList;
                ListWithMaxInd->DelElemInList(LastElemInd);
                ListWithMaxInd->AddElemInList(FindDelIndex);
                if (this->reportTree != nullptr) {
                    List* ListReportTreeWithMaxInd = this->reportTree->SearchInTree(SummDate(OrderArr[LastElemInd]->date)).first->IndexList;
                    ListReportTreeWithMaxInd->DelElemInList(LastElemInd);
                    ListReportTreeWithMaxInd->AddElemInList(FindDelIndex);
                }
            }
            OrderArr[LastElemInd] = nullptr;
            NumElemOrdersArr -= 1;


        }

        // удаление самого узла(когда внутренний кольцевой список пуст)
        if (p->IndexList->h == nullptr) {

            //Нет потомков
            if (p->left == nullptr && p->right == nullptr) {
                if (p == root) root = nullptr;
                delete p;
                return nullptr;
            }
            //Нет левого потомка
            else if (p->left == nullptr) {
                BaseNode* r = p->right;
                if (p == root) root = r;
                delete p;
                return r;
            }
            //Нет правого потомка
            else if (p->right == nullptr) {
                BaseNode* l = p->left;
                if (p == root) root = l;
                delete p;
                return l;
            }
            //Есть оба потомка
            else {
                // Ищем замену: максимальный элемент в левом поддереве
                BaseNode* maxNode = maxLeft(p->left);
                // Очищаем старый (уже пустой) список текущего узла
                delete p->IndexList;

                p->IndexList = maxNode->IndexList;
                p->SetKey(maxNode->GetKey());
                // Отвязываем список от узла-замены. 
                maxNode->IndexList = nullptr;
                // Удаляем узел-замену из левого поддерева
                p->left = RemoveMax(p->left);
                // Возвращаем текущий узел 
                return Rebalanse(p);
            }
        }
        
        
    }
    return Rebalanse(p);


}








unsigned int NodeReportTree::GetKey()
{
    return SummDate(this->key);
}

void NodeReportTree::SetKey(const unsigned int& newKey)
{

    this->key.d = newKey % 100;
    this->key.y = newKey/10000;
    this->key.m = ConvIntToMon((newKey/100)%100);

}

size_t ReportTree::GetCapasityReportArr()
{
    return this->CapacityReportArr;
}

int ReportTree::GetNumElemReportArr()
{
    return this->NumElemReportArr;
}

void ReportTree::ResizeReportArr()
{
    
    CapacityReportArr *= 2;

    DataReport** newArr = new DataReport * [CapacityReportArr];

    for (int i = 0; i < NumElemReportArr; i++)
        newArr[i] = ReportArr[i];

    for (int i = NumElemReportArr; i < CapacityReportArr; i++)
        newArr[i] = nullptr;

    delete[] ReportArr;
    ReportArr = newArr;
}



NodeReportTree::NodeReportTree(Date& key):key(key), BaseNode() {}

void ReportTree::ClearReportArr()
{
    for (int i = 0; i < CapacityReportArr; i++) {
        if (ReportArr[i] != nullptr) {
            delete ReportArr[i];
            ReportArr[i] = nullptr;
        }
    }
    NumElemReportArr = 0;
}
ReportTree::ReportTree(MainTree* mainTree, HashTable* mainHashTable) {
    this->root = nullptr;
    this->CapacityReportArr = 2;
    this->NumElemReportArr = 0;
    this->mainTree = mainTree;
    this->CourierHashTable = mainHashTable;
    ReportArr = new DataReport * [CapacityReportArr];
    for (int i = 0; i < CapacityReportArr; i++) {

        ReportArr[i] = nullptr;
    }
}





DataOrder* ReportTree::GetCellOrderArr(int& i)
{
    return  mainTree->GetCellOrderArr(i);
}

BaseNode* ReportTree::AddInTreeElem(BaseNode* p, DataOrder& data) {

    if (p == nullptr) { // добавление нового узла
        p = new NodeReportTree(data.date);
        int indexMainArr = FindElemInListNodeTree(mainTree->SearchInTree(data.passNum).first, data);
        p->IndexList->AddElemInList(indexMainArr);
        
        if (root == nullptr) root = p;
        return p;
    }
    unsigned int DATEData = SummDate(data.date);
   
    unsigned int DATEDataP = p->GetKey();
    if (DATEData < DATEDataP) {
        p->left = AddInTreeElem(p->left, data);
        FixedHight(p);
    }
    else if (DATEData > DATEDataP) {
        p->right = AddInTreeElem(p->right, data);
        FixedHight(p);
    }
    else {
        int indexMainArr = FindElemInListNodeTree(mainTree->SearchInTree(data.passNum).first, data);
        p->IndexList->AddElemInList(indexMainArr);
        
    }
    return Rebalanse(p);
}


BaseNode* ReportTree::DelInTreeElem(BaseNode* p, DataOrder& data) {

    if (p == nullptr) {
        return nullptr;
    }
    unsigned int DATEData = SummDate(data.date);
    unsigned int DATEDataP = p->GetKey();
    if (DATEData < DATEDataP)
    {
        p->left = DelInTreeElem(p->left, data);
        FixedHight(p);
    }
    else if (DATEData > DATEDataP)
    {
        p->right = DelInTreeElem(p->right, data);
        FixedHight(p);
    }
    else
    {
        int FindDelIndex = FindElemInListNodeTree(mainTree->SearchInTree(data.passNum).first, data);
        if (FindDelIndex != -1)
        {
            p->IndexList->DelElemInList(FindDelIndex);
            
        }
        if (p->IndexList->h == nullptr) {

            //Нет потомков
            if (p->left == nullptr && p->right == nullptr) {
                if (p == root) root = nullptr;
                delete p;
                return nullptr;
            }
            //Нет левого потомка
            else if (p->left == nullptr) {
                BaseNode* r = p->right;
                if (p == root) root = r;
                delete p;
                return r;
            }
            //Нет правого потомка
            else if (p->right == nullptr) {
                BaseNode* l = p->left;
                if (p == root) root = l;
                delete p;
                return l;
            }
            //Есть оба потомка
            else {
                // Ищем замену: максимальный элемент в левом поддереве
                BaseNode* maxNode = maxLeft(p->left);
                // Очищаем старый (уже пустой) список текущего узла
                delete p->IndexList;

                p->IndexList = maxNode->IndexList;
                p->SetKey(maxNode->GetKey());
                // Отвязываем список от узла-замены. 
                maxNode->IndexList = nullptr;
                // Удаляем узел-замену из левого поддерева
                p->left = RemoveMax(p->left);
                // Возвращаем текущий узел 
                return Rebalanse(p);
            }
        }

    }
    return Rebalanse(p);


}

std::pair<BaseNode*, int> ReportTree::SearchInTree(const unsigned int& key) {

    int SearchSteps = 1;
    BaseNode* p = root;
    while (p != nullptr) {
        unsigned int DATEData = key;
        unsigned int DATEDataP = p->GetKey();

        if (DATEData < DATEDataP) {
            p = p->left;
            SearchSteps += 1;
        }
        else if (DATEData > DATEDataP) {
            p = p->right;
            SearchSteps += 1;
        }
        else return { p , SearchSteps };
    }
    return { nullptr, SearchSteps };
}

bool ReportTree::GenerateReport(Date& date, Adres& adres, FIO& fio)
{
    ClearReportArr(); // отчищаем предыдущий отчёт
    bool isGenerate = false;
    BaseNode* FoundDATENode = SearchInTree(SummDate(date)).first;
    if (FoundDATENode != nullptr)
    {
        NodeList* p = FoundDATENode->IndexList->h;

        do {
            p = p->next;
            Adres FoundAdress;
            FoundAdress = GetCellOrderArr(p->Index)->adres;
            if (FoundAdress == adres)
            {
                Cell* FounFIOCell = CourierHashTable->SearchInHashTable(GetCellOrderArr(p->Index)->passNum).first;
                if (FounFIOCell != nullptr)
                {
                    if (CourierHashTable->GetCellCourierArr(FounFIOCell->index)->fio == fio)
                    {
                        ReportArr[NumElemReportArr] = new DataReport;
                        ReportArr[NumElemReportArr]->date = date;
                        ReportArr[NumElemReportArr]->price = GetCellOrderArr(p->Index)->price;
                        ReportArr[NumElemReportArr]->adres = FoundAdress;

                        ReportArr[NumElemReportArr]->fio = CourierHashTable->GetCellCourierArr(FounFIOCell->index)->fio;
                        ReportArr[NumElemReportArr]->transp = CourierHashTable->GetCellCourierArr(FounFIOCell->index)->transp;
                        ReportArr[NumElemReportArr]->passNum = FounFIOCell->key;

                        NumElemReportArr += 1;
                        isGenerate = true;

                    }
                }
            }
            if (NumElemReportArr == CapacityReportArr) ResizeReportArr();

        } while (p != FoundDATENode->IndexList->h);
    }
    return isGenerate;
}





void ReportTree::PrintReport(std::ostream& out)
{
    if (ReportArr)
    {
        for (int i = 0; i < NumElemReportArr; i++)
        {
            if (ReportArr[i] != nullptr) out << ReportArr[i];
        }
    }
}





