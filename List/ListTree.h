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
	void AddElemInList( int data);
	void DelElemInList( int data);
	bool SearchInList( int data);
	
};

class NodeList
{

public:
	NodeList* next;
	int Index;
	NodeList(int& data);
	~NodeList();
};


