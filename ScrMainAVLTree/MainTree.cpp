#include "MainTree.h"
#include"../Stracts//Stracts.h"
#include"../List/ListTree.h"
#include"../ScrHashTable/HashTable.h"
#include"../ChekersInputData/CheckerInputData.h"
#include <map>
#include<locale>
#include<sstream>
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


int SummDate(Date& date) {
    return date.y * 10000 + ConvMonToInt(date.m) * 100 + date.d;
}
int ConvMonToInt(string& m)
{
    map<string, int> months = {
        {"jan", 1}, {"feb", 2}, {"mar", 3},
        {"apr", 4}, {"may", 5}, {"jun", 6},
        {"jul", 7}, {"aug", 8}, {"sep", 9},
        {"oct", 10}, {"nov", 11}, {"dec", 12}
    };

    auto it = months.find(m);
    if (it != months.end())
        return it->second;
    return -1; // если не найдено
}



void MainTree::ResizeArr()
{
    Capacity *= 2;

    DataOrder** newArr = new DataOrder * [Capacity];

    for (int i = 0; i < OrderArrSize; i++)
        newArr[i] = OrderArr[i];

    for (int i = OrderArrSize; i < Capacity; i++)
        newArr[i] = nullptr;

    delete[] OrderArr;
    OrderArr = newArr;
}


int MainTree::GetHight(NodeMainTree* p)
{
    return p == nullptr ? 0 : p->hight;
}
int MainTree::DifferHights(NodeMainTree* p)
{
    return GetHight(p->right) - GetHight(p->left);
}
void MainTree::FixedHight(NodeMainTree* p)
{
    unsigned int hl = GetHight(p->left);
    unsigned int hr = GetHight(p->right);
    p->hight = (hl > hr ? hl : hr) + 1;
}
NodeMainTree::NodeMainTree() {
    hight = 1;
    left = nullptr;
    right = nullptr;
    IndexList = new List();
}

NodeMainTree::~NodeMainTree() {
    delete IndexList;

}

MainTree::MainTree(HashTable* courierHashTable) {

    this->CourierHashTable = courierHashTable;
    this->CourierHashTable->SetOrderTree(this);
    this->root = nullptr;
    this->Capacity = 2;
    this->OrderArrSize = 0;
    this->NumElem = 0;
    this->OrderArr = new DataOrder*[Capacity];
    for (int i = 0; i < Capacity; i++) {
        
        this->OrderArr[i] = nullptr;
    }
}
void MainTree::DeleteTree(NodeMainTree* root) {
    if (root == nullptr)
    {
        return;
    }
    DeleteTree(root->left);
    DeleteTree(root->right);
    delete root;
    return;
    this->root = nullptr;
}

MainTree::~MainTree() {
    
        for (int i = 0; i < OrderArrSize; i++)
            if(OrderArr[i] != nullptr) delete OrderArr[i];
        delete[] OrderArr;
    DeleteTree(root);
}



NodeMainTree* MainTree::RightTurn(NodeMainTree* node) {
    NodeMainTree* q = node->left;
    node->left = q->right;
    q->right = node;
    if (node == root) root = q;
    FixedHight(node);
    FixedHight(q);
    return q;
}

NodeMainTree* MainTree::LeftTurn(NodeMainTree* node) {
    NodeMainTree* p = node->right;
    node->right = p->left;
    p->left = node;
    if (node == root) root = p;
    FixedHight(node);
    FixedHight(p);
    return p;
}
NodeMainTree* MainTree::Rebalanse(NodeMainTree* node) {
    NodeMainTree* p = node;
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


void MainTree::FillArr(std::istream& in)
{
    string line;
    bool inKouriers = true;
    while ((inKouriers == true) && (std::getline(in, line)))
    {
        if (!line.empty())
        {
            std::istringstream iss(line);

            pair <DataOrder, std::string> elem = CheckInputOrder(iss);

            if (CourierHashTable->SearchInHashTable(elem.first.passNum).first != nullptr)
            {
                if (elem.second.empty())
                {
                    AddElemInArr(elem.first);

                }
                // иначе просто ничего не делаем Ч строка пропущена
            }
            else
            {
                // останавливаем ввод так как очередного ключа нет в справочнике " урьеры" 
                DeleteTree(this->root);
                for (int i = 0; i < OrderArrSize; i++)
                {
                    delete OrderArr[i];
                    OrderArr[i] = nullptr;
                }
                inKouriers = false;
            }
        }
    }

}

bool MainTree::AddElemInArr(DataOrder& elem)
{
    if (CourierHashTable->SearchInHashTable(elem.passNum).first)
    {
        if (AddElem(elem) == true)
        {

            if (OrderArrSize == Capacity)
                ResizeArr();

            OrderArr[OrderArrSize - 1] = new DataOrder;
            OrderArr[OrderArrSize - 1]->adres = elem.adres;
            OrderArr[OrderArrSize - 1]->date = elem.date;
            OrderArr[OrderArrSize - 1]->price = elem.price;
            OrderArr[OrderArrSize - 1]->passNum = elem.passNum;
            return true;

        }
        else return false;
    }
    
}

bool MainTree::AddElem(DataOrder& data)
{
    int SizeOld = this->OrderArrSize;
    AddInTreeElem(this->root, data);
    int SizeNew = this->OrderArrSize;
    if (SizeOld != SizeNew) return true;
    else return false;


}

NodeMainTree* MainTree::AddInTreeElem(NodeMainTree* p, DataOrder& data) {


   
    if (p == nullptr) { // добавление нового узла
        p = new NodeMainTree();
        p->key = data.passNum;
        p->IndexList->AddElem(OrderArrSize);
        OrderArrSize += 1;
        NumElem += 1;
        if (root == nullptr) root = p;
        return p;
    }
    unsigned int passNamData = data.passNum;
    unsigned int passNamDataP = p->key;
    if (passNamData < passNamDataP){
        p->left = AddInTreeElem(p->left, data);
        FixedHight(p);
    }
    else if (passNamData > passNamDataP){
        p->right = AddInTreeElem(p->right, data);
        FixedHight(p);
    }
    else{
        int FinnDada = FindElemInList(p, data);
        if (FinnDada == -1)
        {
            p->IndexList->AddElem(OrderArrSize);
            OrderArrSize += 1;
            NumElem += 1;
        }
        else
        {
            delete p;
            p = nullptr;
        }
    }
    return Rebalanse(p);
}





std::pair<NodeMainTree*, int> MainTree::SearchInTree( const unsigned int & passNum) {

    int SearchSteps = 0;
    NodeMainTree* p = root;
    while (p != nullptr) {

        if (passNum < p->key){
            p = p->left;
            SearchSteps += 1;
        }
        else if (passNum > p->key) {
            p = p->right;
            SearchSteps += 1;
        }
        else return {p, SearchSteps };
    }
    return { nullptr, SearchSteps };
}





NodeMainTree* MainTree::maxLeft(NodeMainTree* p) {
    if (p->right == nullptr) return p;
    return maxLeft(p->right);

}
NodeMainTree* MainTree::RemoveMax(NodeMainTree* p)
{
    if (p->right == nullptr)
    {
        return p->left;
    }
    p->right = RemoveMax(p->right);
    return Rebalanse(p);
}

int MainTree::FindElemInList(NodeMainTree* findData, DataOrder& data)
{
    NodeList* p = findData->IndexList->h;
    if (p->next == findData->IndexList->h)
    {
        if (*OrderArr[p->Index] == data) return p->Index;
    }
    else
    {
        do
        {
            p = p->next;
            if (*OrderArr[p->Index] == data) return p->Index;

        } while (p != findData->IndexList->h);

        ;
    }

    return -1;
}




bool  MainTree::DelElemInArr(DataOrder& elem)
{
    if (DelElem(elem) != true)
    {
        return false;
        
    }
    return true;
}


bool MainTree::DelElem(DataOrder& data)
{
    int SizeOld = this->NumElem;
    DelInTreeElem(this->root, data);
    int SizeNew = this->NumElem;
    if (SizeOld != SizeNew) return true;
    else return false;
}

NodeMainTree* MainTree::DelInTreeElem(NodeMainTree* p, DataOrder& data) {

    if (p == nullptr) {
        return nullptr;
    }
    unsigned int passNamData = data.passNum;
    unsigned int passNamDataP = p->key;
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
        int FinnDada = FindElemInList(p, data);
        if (FinnDada != -1)
        {
            p->IndexList->DelElem(FinnDada);
            delete OrderArr[FinnDada];
            OrderArr[FinnDada] = nullptr;
            NumElem -= 1;
        }

        if (p->IndexList->h == nullptr)
        {
            if ((p == root) && (p->left == nullptr) && (p->right == nullptr))
            {
                delete p;
                p = nullptr;
                root = nullptr;
                return p;
            }
            else
            {
                NodeMainTree* q = p->left;
                NodeMainTree* r = p->right;
                if (q == nullptr) return r;
                NodeMainTree* max = maxLeft(q);
                if (p == root) root = max;
                max->left = RemoveMax(q);
                max->right = r;
                delete p;
                return Rebalanse(max);
            }
        }
        
    }
    return Rebalanse(p);


}



void MainTree::PrintTree(NodeMainTree* root, int space, int indent, ostream& out) {
    if (root == nullptr)   // Ѕазовый случай
    {
        return;
    }
    space += indent;
    out << "\n\n\n";
    this->PrintTree(root->right, space, indent, out);  //рекурсивный вызов правого поддерева

    
        NodeList* p = root->IndexList->t;
        do
        {
            p = p->next;
            for (int i = 0; i < space; i++)
                out << " ";
            out << OrderArr[p->Index];
            
        } while (p->next != root->IndexList->h);
    

    this->PrintTree(root->left, space, indent, out);   //рекурсивный вызов левого поддерева


};


void MainTree::PrintOrderArr(std::ostream& out)
{
    for (int i = 0; i < OrderArrSize; i++)
    {
        if (OrderArr[i] != nullptr)
            out << OrderArr[i];
        else
            out << "nullptr" << endl;

    }
}


