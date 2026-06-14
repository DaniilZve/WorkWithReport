#include "ReportTree.h"
#include"../List/ListTree.h"
#include"../ScrMainAVLTree/MainTree.h"
#include"../ScrHashTable/HashTable.h"
#include"../Stracts/Stracts.h"
#include <map>
#include <string>
#include<locale>
#include<sstream>

using namespace std;
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
        << " " << data->price
        << " " << data->fio.f << " " << data->fio.i << " " << data->fio.o
        << " " << data->transp.brand << data->transp.model
        << " " << data->price << endl;

};

void ReportTree::ResizeReportArr()
{
    CapacityReportArr *= 2;

    DataReport** newArr = new DataReport * [CapacityReportArr];

    for (int i = 0; i < SizeReportArr; i++)
        newArr[i] = ReportArr[i];

    for (int i = SizeReportArr; i < CapacityReportArr; i++)
        newArr[i] = nullptr;

    delete[] ReportArr;
    ReportArr = newArr;
}


int ReportTree::GetHight(NodeReportTree* p)
{
    return p == nullptr ? 0 : p->hight;
}
int ReportTree::DifferHights(NodeReportTree* p)
{
    return GetHight(p->right) - GetHight(p->left);
}
void ReportTree::FixedHight(NodeReportTree* p)
{
    unsigned int hl = GetHight(p->left);
    unsigned int hr = GetHight(p->right);
    p->hight = (hl > hr ? hl : hr) + 1;
}
NodeReportTree::NodeReportTree() {
    hight = 1;
    left = nullptr;
    right = nullptr;
    IndexList = new List();
}

NodeReportTree::~NodeReportTree() {
    delete IndexList;

}

ReportTree::ReportTree(MainTree* mainTree, HashTable* mainHashTable) {
    this->ReportTreeroot = nullptr;
    this->CapacityReportArr = 2;
    this->SizeReportArr = 0;
    this->NumElem = 0;
    this->mainTree = mainTree;
    this->mainHashTable = mainHashTable;
    ReportArr = new DataReport * [CapacityReportArr];
    for (int i = 0; i < CapacityReportArr; i++) {

        ReportArr[i] = nullptr;
    }
}

ReportTree::~ReportTree() {

    for (int i = 0; i < SizeReportArr; i++)
        delete ReportArr[i];
    delete[] ReportArr;
    DeleteReportTree(ReportTreeroot);
}

void ReportTree::DeleteReportTree(NodeReportTree* root) {
    if (root == nullptr)
    {
        return;
    }
    DeleteReportTree(root->left);
    DeleteReportTree(root->right);
    delete root;
}


NodeReportTree* ReportTree::RightTurn(NodeReportTree* node) {
    NodeReportTree* q = node->left;
    node->left = q->right;
    q->right = node;
    if (node == ReportTreeroot) ReportTreeroot = q;
    FixedHight(node);
    FixedHight(q);
    return q;
}

NodeReportTree* ReportTree::LeftTurn(NodeReportTree* node) {
    NodeReportTree* p = node->right;
    node->right = p->left;
    p->left = node;
    if (node == ReportTreeroot) ReportTreeroot = p;
    FixedHight(node);
    FixedHight(p);
    return p;
}
NodeReportTree* ReportTree::Rebalanse(NodeReportTree* node) {
    NodeReportTree* p = node;
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

int ReportTree::FindElemInListMainTree(NodeMainTree* findData, DataOrder* data)
{
    NodeList* p = findData->IndexList->h;
    if (p->next == findData->IndexList->h)
    {
        if (*mainTree->OrderArr[p->Index] == *data) return p->Index;
    }
    else
    {
        while (p->next != findData->IndexList->h)
        {
            if (*mainTree->OrderArr[p->Index] == *data) return p->Index;
            p = p->next;
        }
        if (*mainTree->OrderArr[p->Index] == *data) return p->Index;
    }

    return -1;
}

void ReportTree::FillReoortTree()
{
    for (int i = 0; i < mainTree->OrderArrSize; i++)
    {

        if (mainTree->OrderArr[i] != nullptr)
            AddInReportTreeElem(this->ReportTreeroot, mainTree->OrderArr[i]);
    }
}


NodeReportTree* ReportTree::AddInReportTreeElem(NodeReportTree* p, DataOrder* data) {

    if (p == nullptr) { // добавление нового узла
        p = new NodeReportTree();
        p->key = data->date;
        int indexMainArr = FindElemInListMainTree(mainTree->SearchInTree(data->passNum).first, data);
        p->IndexList->AddElem(indexMainArr);
        NumElem += 1;
        if (ReportTreeroot == nullptr) ReportTreeroot = p;
        return p;
    }
    unsigned int DATEData = SummDate(data->date);
    unsigned int DATEDataP = SummDate(p->key);
    if (DATEData < DATEDataP) {
        p->left = AddInReportTreeElem(p->left, data);
        FixedHight(p);
    }
    else if (DATEData > DATEDataP) {
        p->right = AddInReportTreeElem(p->right, data);
        FixedHight(p);
    }
    else {
        int indexMainArr = FindElemInListMainTree(mainTree->SearchInTree(data->passNum).first, data);
        p->IndexList->AddElem(indexMainArr);
        NumElem += 1;
    }
    return Rebalanse(p);
}


bool ReportTree::GenerateReport(Date& date, Adres& adres, FIO& fio)
{
    bool isGenerate = false;
    FillReoortTree();
    NodeReportTree* FoundDATENode = SearchElemInReportTree(date);
    if (FoundDATENode != nullptr)
    {
        NodeList* p = FoundDATENode->IndexList->h;
        
        do {
            p = p->next;
            Adres FoundAdress;
            FoundAdress = mainTree->OrderArr[p->Index]->adres;
            if (FoundAdress == adres)
            {
                Cell* FounFIOCell = mainHashTable->SearchInHashTable(mainTree->OrderArr[p->Index]->passNum).first;
                if (FounFIOCell != nullptr)
                {
                    if (mainHashTable->CourierArr[FounFIOCell->index]->fio == fio)
                    {
                        ReportArr[SizeReportArr] = new DataReport;
                        ReportArr[SizeReportArr]->date = FoundDATENode->key;
                        ReportArr[SizeReportArr]->price = mainTree->OrderArr[p->Index]->price;
                        ReportArr[SizeReportArr]->adres = FoundAdress;

                        ReportArr[SizeReportArr]->fio = mainHashTable->CourierArr[FounFIOCell->index]->fio;
                        ReportArr[SizeReportArr]->transp = mainHashTable->CourierArr[FounFIOCell->index]->transp;
                        ReportArr[SizeReportArr]->passNum = FounFIOCell->key;

                        SizeReportArr += 1;
                        isGenerate = true;

                    }
                }
            } 
            if (SizeReportArr == CapacityReportArr) ResizeReportArr();
            
        } while (p != FoundDATENode->IndexList->h);
    }
    return isGenerate;
}



NodeReportTree* ReportTree::SearchElemInReportTree(Date& date) {

    int SearchSteps = 0;
    NodeReportTree* p = ReportTreeroot;
    while (p != nullptr) {
        unsigned int DATEData = SummDate(date);
        unsigned int DATEDataP = SummDate(p->key);

        if (DATEData < DATEDataP) {
            p = p->left;
            SearchSteps += 1;
        }
        else if (DATEData > DATEDataP) {
            p = p->right;
            SearchSteps += 1;
        }
        else return p;
    }
    return nullptr;
}


void ReportTree::PrintReportTree(NodeReportTree* root, int space, int indent, ostream& out) {
    if (root == nullptr)   // Базовый случай
    {
        return;
    }
    space += indent;
    out << "\n\n\n";
    this->PrintReportTree(root->right, space, indent, out);  //рекурсивный вызов правого поддерева


    NodeList* p = root->IndexList->t;
    do
    {
        p = p->next;
        for (int i = 0; i < space; i++)
            out << " ";
        out << root->key.d<<" "<< root->key.m << " " << root->key.y << endl;

    } while (p->next != root->IndexList->h);


    this->PrintReportTree(root->left, space, indent, out);   //рекурсивный вызов левого поддерева


};

void ReportTree::PrintReport(std::ostream& out)
{
    for (int i = 0; i < SizeReportArr; i++)
    {
            out << ReportArr[i];
    }
}



