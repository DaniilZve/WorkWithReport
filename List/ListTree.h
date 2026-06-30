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
	void AddElemInList( int& ind);
	void DelElemInList( int& ind);
	bool SearchInList( int& ind);
	
};

class NodeList
{

public:
	NodeList* next;
	int Index;
	NodeList(int& ind);
	~NodeList();
};


