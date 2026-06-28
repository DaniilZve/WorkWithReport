#include "ListTree.h"

NodeList::NodeList(int& data) {
	this->Index = data;
	this->next = nullptr; 
}

NodeList::~NodeList() {
}

List::List() {
	this->h = nullptr;
	this->t = nullptr;
}

List::~List() {
	if (h == nullptr) return;

	NodeList* p = h;
	t->next = nullptr; 
	while (p != nullptr) {
		NodeList* nextNode = p->next;
		delete p;
		p = nextNode;
	}
	h = nullptr;
	t = nullptr;
}

// Вставка с сохранением порядка ПО УБЫВАНИЮ
void List::AddElemInList(int data) {
	NodeList* NewNode = new NodeList(data);

	// Список пуст
	if (h == nullptr) {
		h = NewNode;
		t = NewNode;
		h->next = t;
		
	}
	else
	{
		// Новый элемент больше головы (вставка в самое начало)
		if (data > h->Index) {
			NewNode->next = h;
			h = NewNode;
			t->next = h; // Поддерживаем кольцо
			
		}
		else
		{

			// Вставка в середину или в конец
			NodeList* prev = h;
			NodeList* curr = h->next;
			while (curr != h && curr->Index > data) {
				prev = curr;
				curr = curr->next;
			}

			// Вставляем узел между prev и curr
			prev->next = NewNode;
			NewNode->next = curr;

			// Если вставили в самый конец
			if (prev == t) {
				t = NewNode;
			}
		}
		
	}
	
}

void List::DelElemInList(int data) {
	if (h != nullptr)
	{

		NodeList* p = h;
		NodeList* q = t;

		// Поиск элемента
		while (p->Index != data && p->next != h) {
			q = p;
			p = p->next;
		}

		// Если элемент найден
		if (p->Index == data)
		{
			// Если элемент всего один в списке
			if (h == t) {
				h = nullptr;
				t = nullptr;
			}
			// Если удаляем голову
			else if (p == h) {
				h = h->next;
				t->next = h;
			}
			// Если удаляем хвост
			else if (p == t) {
				t = q;
				t->next = h;
			}
			// Удаление из середины
			else {
				q->next = p->next;
			}
			delete p;
		}
	}
}

bool List::SearchInList(int data)
{
	NodeList* p = this->h;
	while (p != h && p->Index != data);
	{
		p = p->next;
	} 
	if (p->Index == data) return true;
	else return false;
}