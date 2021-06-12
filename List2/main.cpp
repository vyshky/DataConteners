#include <iostream>
using namespace std;
#define tab "\t"


class List
{
	////////////////////////////////////////////////////////////////////////
	unsigned int size;

public:	
	class Element
	{
		int Data;
		Element* pNext;
		Element* pPrev;
	public:

		Element(int Data, Element* pNext = nullptr, Element* pPrev = nullptr)
		{
			this->Data = Data;
			this->pNext = pNext;
			this->pPrev = pPrev;
			cout << "EConstructro:\t" << this << endl;
		}
		~Element()
		{
			cout << "EDestructor:\t" << this << endl;
		}
		friend class List;
		friend class Iterator;
	}*Head, * Tail;//Сразу же после описания класса объявляем два указателя на объекты этого класса -  указатели на начальный и конечный Элемент

	class Iterator
	{
		Element* Temp;
		//Этот класс просто оборачивает указатель на Element,что позваоляет нам перегружать операции для указателя на элемент
	public:
		Iterator(Element* Temp = nullptr)
		{
			this->Temp = Temp;
			cout << "ITConstruct:\t" << this << endl;
		}
		Iterator(const Iterator& other)
		{
			this->Temp = other.Temp;
			cout << "ITCopyconstructor:\t" << this << endl;
		}
		~Iterator()
		{
			cout << "ITDestructor:\t" << this << endl;
		}
		Iterator& operator++()//prefix increment
		{
			this->Temp = Temp->pNext;
			return *this;
		}
		Iterator& operator++(int)//posfix increment
		{
			Iterator old = *this;
			this->Temp = Temp->pNext;
			return old;
		}
		bool operator==(const Iterator& other)const
		{
			return this->Temp == other.Temp;
		}
		bool operator!=(const Iterator& other)const
		{
			return this->Temp != other.Temp;
		}
		const int& operator*()const
		{
			return Temp->Data;
		}
		int& operator*()
		{
			return Temp->Data;
		}
	};

	List()
	{
		Head = Tail = nullptr;
		size = 0;
		cout << "LConstructor:\t" << this << endl;
	}
	List(const initializer_list<int> il) :List()
	{
		//Head = Tail = nullptr;
		cout << typeid(il.begin()).name() << endl;
		for (const int* IT = il.begin(); IT != il.end(); ++IT)
		{
			this->push_back(*IT);
		}
	}
	~List()
	{
		while (Head) pop_back();
		cout << "LDestructor:\t" << this << endl;
	}
	//       Adding elements:
	void push_front(int Data)
	{
		if (Head == nullptr && Tail == nullptr)
		{
			Head = Tail = new Element(Data);
			++size;
			return;
		}

		/*Element* New = new Element(Data);
		New->pNext = Head;
		Head->pPrev = New;
		Head = New;
		++size;*/

		Head = new Element(Data, Head);
		Head->pNext->pPrev = Head;
		++size;
	}
	void push_back(int Data)
	{
		if (Head == nullptr && Tail == nullptr)
		{
			push_front(Data);
			return;
		}
		/*Element* New = new Element(Data);
		New->pPrev = Tail;
		Tail->pNext = New;
		Tail = New;
		++size;*/

		Tail->pNext = new Element(Data, nullptr, Tail);
		Tail = Tail->pNext;
		++size;
	}
	//      Delete elements:
	void pop_front()
	{
		if (Head == nullptr && Tail == nullptr)return;
		if (Head->pNext == nullptr)
		{
			delete Head;
			Head = Tail = nullptr;
			--size;
			return;
		}
		Element* to_del = Head;
		Head = Head->pNext;
		Head->pPrev = nullptr;
		delete to_del;
		--size;
	}
	void pop_back()
	{
		if (Head == nullptr && Tail == nullptr)return;
		if (Tail->pPrev == nullptr)
		{
			delete Head;
			Head = Tail = nullptr;
			--size;
			return;
		}
		Element* to_del = Tail;
		Tail = Tail->pPrev;
		Tail->pNext = nullptr;
		delete to_del;
		--size;
	}
	//     Insert element:
	void insert(int Index, int Data)
	{
		if (Index == 0)
		{
			push_front(Data);
			return;
		}

		if (Index > size)
		{
			push_back(Data);
			return;
		}
		bool change_operator = (Index <= size / 2) ? true : false;
		Element* Temp = (change_operator) ? Head : Tail;

		for (int i = (change_operator) ? 0 : size; i != Index - 1;)
		{	/* i = (change_operator) ? ++i         : --i;
			Temp = (change_operator) ? Temp->pNext : Temp->pPrev;*/
			if (change_operator)
			{
				Temp = Temp->pNext;
				++i;
			}
			else
			{
				Temp = Temp->pPrev;
				--i;
			}
		}
		Element* New = new Element(Data);
		New->pNext = Temp->pNext;
		New->pPrev = Temp;
		Temp->pNext->pPrev = New;
		Temp->pNext = New;
		++size;
	}

	void erase(int Index)
	{
		if (Index == 0)
		{
			pop_front();
			return;
		}
		if (Index >= size)
		{
			pop_back();
			return;
		}
		bool change_operator = (Index <= size / 2) ? true : false;
		Element* Temp = (change_operator) ? Head : Tail;
		for (int i = (change_operator) ? 0 : size; i != Index - 1;)
		{
			if (change_operator)
			{
				Temp = Temp->pNext;
				++i;
			}
			else
			{
				Temp = Temp->pPrev;
				--i;
			}
		}
		Temp->pNext = Temp->pNext->pNext;
		delete Temp->pNext->pPrev;
		Temp->pNext->pPrev = Temp;
		--size;
	}
	//         RANGE_BASED_ARRAY	
	Iterator begin()
	{
		return Head;
	}
	const Iterator begin()const
	{
		return Head;
	}

	Iterator end()
	{
		return nullptr;
	}

	const Iterator end()const
	{
		return nullptr;
	}

	void print()
	{
		cout << endl;
		cout << " pPrev" << tab << tab << "  Temp" << tab << "  Data" << tab << " pNext" << endl;
		cout << "________________________________________" << endl;
		for (Element* Temp = Head; Temp; Temp = Temp->pNext)
		{
			cout << Temp->pPrev << tab << Temp << " = " << Temp->Data << tab << Temp->pNext << endl;
		}
		cout << endl << "Количество элементов списка:" << tab << tab << size << endl;
	}
};

/////////////////////////////////////////////////////////////////////////

//#define POP_ELEMENT
//#define INSERT_ELEMENT
//#define ERASE_ELEMENT
#define RANGE_BASED_ARRAY
void main()
{
	setlocale(LC_ALL, "Russian");
	/*int n;
	cout << "Введите размер списка: "; cin >> n;
	List list;
	for (int i = 0; i < n; i++)
	{
		list.push_back(rand() % 100);
	}*/

#ifdef POP_ELEMENT
	list.print();
	list.pop_front();
	list.pop_front();
	list.pop_front();
	list.pop_front();
	list.pop_front();
	list.pop_front();
	list.pop_front();
	list.pop_back();
	list.pop_back();
	list.pop_back();
	list.pop_back();
	list.pop_back();
	list.pop_back();
	list.pop_back();
	list.pop_back();
	list.print();
#endif // DEBUG

#ifdef INSERT_ELEMENT
	list.print();
	list.insert(0, 50);
	list.insert(0, 50);
	list.insert(0, 50);
	list.insert(0, 50);
	list.print();
#endif // INSERT_ELEMENT


#ifdef ERASE_ELEMENT
	list.print();
	list.erase(0);
	list.print();
	list.erase(5);
	list.print();
#endif // ERASE_ELMENT

#ifdef RANGE_BASED_ARRAY
	List list2{ 3,5,8,13,21 };

	for (int& i : list2)
	{
		cout << typeid(i).name() << tab;
	}

	for (List::Iterator i = list2.begin(), j = list2.end(); i != j; ++i)
	{
		cout << *i << tab;
	}

#endif // RANGE_BASED_ARRAY


}
