#pragma once
#include"../Stracts/Stracts.h"

class NodeList;
class List
{
public:
	NodeList* h;
	NodeList* t;
	List();
	~List();
	List* AddElem( int& data);
	List* DelElem( int& data);
};

class NodeList
{

public:
	NodeList* next;
	int Index;
	NodeList(int& data);
	~NodeList();
};


