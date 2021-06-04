#include<iostream>
using namespace std;
#define tab "\t"
//#include <initializer_list>

class ForwardList;
ForwardList operator+(const ForwardList& left, const ForwardList& right);

class Element
{
	int Data;
	Element* pNext;
	static int count;
public:
	Element* get_pNext()const
	{
		return pNext;
	}
	int get_Data()const
	{
		return Data;
	}
	void set_Data(int Data)
	{
		this->Data = Data;
	}
	void set_pNext(Element* pNext)
	{
		this->pNext = pNext;
	}
	Element(int Data, Element* pNext = nullptr)
	{
		this->Data = Data;   //Значение Элемента
		this->pNext = pNext; //Указатель на следующий элемент
		count++;
		cout << "EConstructor:\t" << this << endl;
	}
	~Element()
	{
		count--;
		cout << "EDestructor:\t" << this << endl;
	}
	friend class ForwardList;
};


int Element::count = 0;

class ForwardList :public initializer_list<int>
{
	Element* Head;// Адресс начального элемента
	unsigned int size;//Cодержит размер списка	
public:

	Element* get_Head()const
	{
		return Head;
	}
	unsigned int get_size()const
	{
		return size;
	}
	void set_Head(Element* Head)
	{
		this->Head = Head;
	}
	ForwardList()
	{
		Head = nullptr;//Eсли голова указывает на 0, то список пуст
		size = 0;
		cout << "LConstructor:\t" << this << endl;
	}
	ForwardList(const ForwardList& other)
	{
		this->Head = nullptr;//Eсли голова указывает на 0, то список пуст
		this->size = 0;
		Element* Temp = other.Head;//Итератор Temp будет проходить по списку other
		while (Temp)
		{
			this->push_back(Temp->Data);// Каждый элемент списка other добaвляем в конец нашего списка
			Temp = Temp->pNext;     //Переход на следующий элемент
		}
		cout << "CopyListConstructor:" << this << endl;
	}

	ForwardList(const initializer_list<int>& Data)
	{
		this->Head = 0;
		this->size = 0;
		/*for (const int* Iterator = Data.begin(); Iterator != Data.end(); Iterator++)
		{
			this->push_back(*Iterator);
		}*/
		for (auto i : Data)
		{
			this->push_back(i);
		}
	}

	//          MoveConstructor
	ForwardList(ForwardList&& other)
	{
		Head = other.Head;
		size = other.size;
		other.Head = nullptr;
		cout << "MoveListConstructor:" << this << endl;
	}
	~ForwardList()
	{
		while (Head)pop_front();
		cout << "LDestructor:\t" << this << endl;
	}

	// Operators:
	ForwardList& operator=(const ForwardList& other)
	{
		if (this == &other)return *this;
		while (Head)pop_front();
		Element* Temp = other.Head;//Создаем итератор, который будет проходить проходить по списку
		while (Temp)
		{
			this->push_back(Temp->Data);
			Temp = Temp->pNext;
		}
		cout << "CopyAssignment:\t" << this << endl;
		return *this;
	}

	ForwardList& operator=(const initializer_list<int>& Data)
	{
		this->Clear();

		for (auto i : Data)
		{
			this->push_back(i);
		}
		return *this;
	}

	//////////////////////////////////////////////	
	//             MoveMethod
	ForwardList& operator=(ForwardList&& other)
	{
		if (this == &other)return *this;
		this->Clear();
		this->Head = other.Head;
		this->size = other.size;
		other.Head = nullptr;
		return *this;
	}
	///////////////////////////////////////////////

	ForwardList& operator+=(const ForwardList& other)
	{
		return *this = *this + other;
	}

	// Adding elements:
	void push_front(int Data)
	{
		////1)Создаем новый элемент
		//Element* New = new Element(Data);
		////2)Привязываем новый элемент к списку
		//New->pNext = Head;
		////3)Говорим, что новый элемент будет начальным элементом списка:
		//Head = New;

		Head = new Element(Data, Head);
		size++;
	}

	void push_back(int Data)
	{
		if (Head == nullptr)
		{
			push_front(Data);
			return;
		}
		//1)Создаем новый элемент
		Element* New = new Element(Data);
		//2)Доходим до конца списка
		Element* Temp = Head;
		while (Temp->pNext)
		{
			Temp = Temp->pNext;
		}
		//3)Прикрепляем новый элемент к последнему
		Temp->pNext = New;
		size++;
	}

	//erasing elements:

	void pop_front()
	{
		//1)Запоминаем адресс удаляемого элемента:
		Element* to_del = Head;
		//2)Исключаем удаляемый элемент из списка:
		Head = Head->pNext;
		//3)Удаляем элемент из памяти
		delete to_del;
		size--;
	}

	void pop_back()
	{
		//1) Доходим до предпоследнего элемента
		Element* Temp = Head;
		while (Temp->pNext->pNext)// в конце остается 1 элемент ,который указывает на null
		{
			Temp = Temp->pNext;
		}
		//2) Удаляем последний элемент из памяти
		delete Temp->pNext; // переходим с предпоследнего элемента на последный , и удаляем его
		//3) Зануляем указатель на последний элемент
		Temp->pNext = nullptr;

		//Мое решение
		/*Element* Temp = Head;
		Element* Temp2 = Head;
		while (Temp->pNext)
		{
			Temp = Temp->pNext;
		}
		while (Temp2->pNext != Temp)
		{
			Temp2 = Temp2->pNext;
		}
		delete Temp;
		Temp2->pNext = nullptr;	*/
		size--;
	}

	void insert(int Index, int Data)
	{
		if (Index == 0)
		{
			this->push_front(Data);
			return;
		}
		if (Index > size)
		{
			return;
		}
		////1)Создаем новый элемент
		//Element* New = new Element(Data);
		////2) Доходим до нужного элемента
		//Element* Temp = Head;
		//for (int i = 0; i < Index - 1; i++)
		//{
		//	Temp = Temp->pNext;
		//}
		////3)Вставляем нужный элемент в нужную позицию
		//New->pNext = Temp->pNext;
		//Temp->pNext = New;

		//1) Доходим до нужного элемента
		Element* Temp = Head;
		for (int i = 0; i < Index - 1; i++)
		{
			Temp = Temp->pNext;
		}
		//2)Создаем новый элемент
		Element* New = new Element(Data, Temp->pNext);
		//3)Вставляем нужный элемент в нужную позицию		
		Temp->pNext = New;
		size++;
	}

	void erase(int Index)
	{
		if (Index == 0)
		{
			this->pop_front();
			return;
		}
		if (Index > size - 1)
		{
			return;
		}
		//1)Находим индекс для удаления элемента
		Element* Temp = Head;
		for (int i = 0; i < Index - 1; i++)
		{
			Temp = Temp->pNext;
		}

		//2)скопировать pNext следующего элемента
		Element* to_del = Temp->pNext;//Копируем Удаляемый элемент Temp2 = index

		//3)Заменяем удаляемый элемент следующим элементом
		Temp->pNext = Temp->pNext->pNext;//Копируем pNext удаляемого элемента (index.pNext - 1 = index.pNext + 1)
		//Было A->pNext => B    ,   B->pNext => C;
		//(A.pNext = B.pNext)
		// Теперь A->pNext => C;

		//Temp = index - 1;  //Элемент находится перед удаляемым(A - Указывает на B)
		//Temp->pNext;       //Удаляемый элемент                (B - Указывает на С)
		//Temp->pNext-pNext; //Следующий элемент                (C - Указывает на n....)

		//4)Удалить эелемент
		delete to_del; // Удаляем index
		size--;
	}

	void Clear()
	{
		Element* to_del;
		while (Head)
		{
			to_del = Head;
			Head = Head->pNext;
			delete to_del;
			size--;
		}
	}

	//  Methods:
	void print()const
	{
		//Element* Temp = Head; //Temp - Итератор 
		////Итератор - это указатель ,при помощи которого
		////при помощи которого можно получить доступ к элементам структуры данных

		//while (Temp)
		//{
		//	cout << Temp << tab << Temp->Data << tab << Temp->pNext << endl;
		//	Temp = Temp->pNext;// Переход на следующий элемент
		//}

		for (Element* Temp = Head; Temp; Temp = Temp->pNext)
		{
			cout << Temp << tab << Temp->Data << tab << Temp->pNext << endl;
		}

		cout << "Колчество элементов в списке: " << size << endl;
		cout << "Общее количество элементов: " << Element::count << endl;
	}


};

ForwardList operator+(const ForwardList& left, const ForwardList& right)
{
	cout << "LIST_left" << endl;
	ForwardList list = left; //Элементы скопируются при создании листа
	Element* Temp = left.get_Head();
	cout << "LIST_right" << endl;

	Temp = right.get_Head();
	while (Temp)
	{
		list.push_back(Temp->get_Data());// Каждый элемент списка other добaвляем в конец нашего списка
		Temp = Temp->get_pNext();     //Переход на следующий элемент
	}
	return list;
}


void main()
{
	setlocale(LC_ALL, "rus");
	int n;
	cout << "Введите колчество элементов списка: "; cin >> n;
	ForwardList list;
	for (int i = 0; i < n; i++)
	{
		list.push_back(rand() % 100);
	}
	list.print();
	//list.push_back(123);
	//list.print();
	//list.pop_front();
	//list.print();
	//list.pop_back();
	//list.print();

	//int index;
	//int value;
	//cout << "Введите индекс добавляемого элемента: "; cin >> index;
	//cout << "Введите значение добавляемого элемента: "; cin >> value;
	//list.insert(index, value);
	//list.print();

	//cout << "Введите индекс удаляемого элемента: "; cin >> index;
	//list.erase(index);
	//list.print();

	/*ForwardList list2 = list;
	list2.print();*/

	//ForwardList list3;
	//list3 = list2;
	//list3 = list;
	//list3.print();

	cout << "------------------------------------" << endl;
	ForwardList list4 = list + list;
	list4.print();


	ForwardList list_arr{ 1,2,3,4,999 };
	list_arr = { 99,4,3,2,1 };
	list_arr.print();
}