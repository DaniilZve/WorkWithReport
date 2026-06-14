#include"ListTree.h"



NodeList::NodeList(int& data) {
	this->Index = data;
}
NodeList::~NodeList() {

}
List::List() {
	this->h = nullptr;
	this->t = nullptr;

}
List:: ~List() {
	NodeList* p = h;
	while (p != t)
	{
		h = p->next;
		delete p;
		t->next = h;
		p = h;
	}
	delete p;
	
}

List* List::AddElem(int& data) {

	if (h == nullptr) {
		h = new NodeList(data);
		t = h;
		h->next = t;
	}
	else
	{
		NodeList* NewNode = new NodeList(data);
		t->next = NewNode;
		t = NewNode;
		NewNode->next = h;
	}
	return this;
}
List* List::DelElem(int& data) {

	if (h != nullptr) {
		NodeList* p = h;
		NodeList* q = h;
		do  { // движение до нужного элемента или до хвоста
			q = p;
			p = p->next;
		} while ((p != h) && (p->Index != data));

		if ((p == h) && (p->next == h) && (p->Index == data)) { // если элемент всего один 
			delete h;
			h = nullptr;
			t = nullptr;
			return nullptr;
		}
		else if ((p == h) && (p->next != h) && (p->Index == data)) // если удаляем голову 
		{
			h = p->next;
			t->next = h;
			delete p;
		}
		else if ((p == t) && (p->Index == data)) // если удаляем хвост 
		{
			t = q;
			t->next = p->next;
			delete p;
		}
		else // если удаляем в середине 
		{
			q->next = p->next;
			delete p;
		}

	}
	return this;
}

