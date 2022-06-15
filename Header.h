#pragma once
#include <iostream>
#include <vector>
#include <tuple>
#include <exception>
#include <cmath>

using namespace std;

enum class status
{
	free, occupied, deleted
};

std::ostream& operator << (std::ostream& out, const status& t)
{
	switch (t) {
	case status::free: return (out << "Free");
	case status::occupied: return (out << "Occupied");
	case status::deleted: return (out << "Deleted");
	}
	return (out);
}

template <typename T>  // концепт, определяющий множество корректных объектов для класса  SimpleHashTable
concept  Hashable = requires(T object)  // объект должен быть приводимым к int, для получения ключа хэширования и конструированным по умолчанию
{
	{(int)object } -> convertible_to<int>;
	requires is_default_constructible<T>::value;
};

template<typename T>
class UniversalContainer      //Основной интерфейс контейнера
{
public:
	virtual ~UniversalContainer() = default;

	virtual void insert(T) = 0;
	virtual void erase(T) = 0;
};


template <Hashable T>
class SimpleHashTable  // Контейнер - хэш таблица
{

	struct cell  // ячейка таблицы
	{
		T data;
		status cell_status;
		cell(T data_) : data{ data_ } { cell_status = status::occupied; }
		cell() { cell_status = status::free; }
	};

	cell* table;
	int size;;
protected:
	template<class Iter>  // класс итератор
	class NodeIterator
	{
		friend struct SimpleHashTable::cell;
		friend class SimpleHashTable;
	public:
		typedef Iter iterator_type;
		typedef std::input_iterator_tag iterator_category;
		typedef iterator_type value_type;
		typedef ptrdiff_t difference_type;
		typedef iterator_type& reference;
		typedef iterator_type* pointer;

		iterator_type* value;

	private:
		NodeIterator(Iter* p) : value{ p } { } //закрытый конструктор

	public:
		NodeIterator(const NodeIterator& it) : value{ it.value } {} // конструктор копирования 

		bool operator !=(NodeIterator const& other) const
		{
			return value != other.value;
		}
		bool operator ==(NodeIterator const& other) const
		{
			return value == other.value;
		}
		typename NodeIterator::reference operator*() const
		{
			return *value;
		}
		NodeIterator& operator++()
		{
			if (value)
			{
				value++;
				return *this;
			}
		}
	};

	//////// основные закрытые матоды
	int search_index(T item)
	{
		int try_counter{ 1 };
		int key = mod_hash_function((int)item);
		while (table[key].cell_status != status::free)
		{
			if (table[key].data == item)
				return key;
			else
				key = (mod_hash_function(key) + try_counter) % size;
		}
		return -1;
	}

	bool is_it_prime(int n) {
		for (auto i = 2; i <= sqrt(n); i++) {
			if (n % i == 0)
				return false;
		}
		return true;
	}

	bool is_table_has_enough_space()
	{
		for (auto i = 0; i < size; i++)
			if (table[i].cell_status == status::free)
				return true;
		return false;
	}

	bool is_that_cell_is_free(int key)
	{
		if (table[key].cell_status == status::free || table[key].cell_status == status::deleted)
			return true;
		else
			return false;
	}

	auto mod_hash_function(int key)
	{
		return key % size;
	}

	int collision(int key)
	{
		int new_key;
		int collision_try = 1;
		do
		{
			new_key = (mod_hash_function(key) + collision_try) % size;
			collision_try++;
		} while (!(table[new_key].cell_status == status::free || table[new_key].cell_status == status::deleted));
		return new_key;
	}


public:
	typedef NodeIterator<cell> iterator;
	typedef NodeIterator<const cell> const_iterator;
	iterator begin()
	{
		return table;
	}

	iterator end()
	{
		return table + size;
	}

	const_iterator begin() const
	{
		return table;
	}

	const_iterator end() const
	{
		return table + size;
	}


	SimpleHashTable(int size_) : size{ size_ }
	{
		if (!is_it_prime(size_))
			throw logic_error("Number isn't prime");
		table = new cell[size_]{ cell{} };
	}
	SimpleHashTable() : size{ 101 } {}
	~SimpleHashTable()
	{
		delete[] table;
	}

	void table_print()
	{
		for (auto i = 0; i < size; i++)
			if (table[i].cell_status == status::occupied)
				cout << i << ")" << " [" << table[i].data << "]" << endl;
			else if (table[i].cell_status == status::free)
				cout << i << ")" << " Free" << endl;
			else
				cout << i << ")" << " Deleted" << endl;
		cout << endl;
	}


	int add_element(T item)
	{

		if (is_table_has_enough_space())
		{
			if (search_index(item) != -1)
				return -1;

			int key = mod_hash_function((int)item);
			if (!is_that_cell_is_free(key))
			{
				int correct_key = collision(key);
				table[correct_key].data = item;
				table[correct_key].cell_status = status::occupied;
			}
			else
			{
				table[key].data = item;
				table[key].cell_status = status::occupied;
			}
			return 0;
		}
		else
			return -1;
	}

	int delete_element(T item)
	{
		int index_to_delete = search_index(item);
		if (index_to_delete != -1)
		{
			table[index_to_delete].cell_status = status::deleted;
			return 1;
		}
		else
			return -1;
	}
};



template<typename T> // Вспомогательный контейнер типа Stack с поддержкой извлечения выбранного элемента 
class Stack : public UniversalContainer<T>
{
	class Node
	{
	public:
		T data;
		Node* next;
		Node* pred;
		Node(T data_, Node* pred_, Node* next_ = nullptr) : data{ data_ }, pred{ pred_ }, next{ next_ } {}
		friend ostream& operator << (ostream& out, Node& object)
		{
			out << object.data;
			return out;
		}
	};

	Node* head;
	Node* tail;

	Node* search(T data)
	{
		Node* temp = head;
		while (temp != nullptr)
		{
			if (temp->data == data)
				return temp;
		}
		return nullptr;
	}
public:
	Stack()
	{
		head = nullptr;
		tail = nullptr;
	}

	void insert(T data) override
	{
		if (!head)
		{
			head = new Node(data, nullptr);
			tail = head;
		}
		else
		{
			Node* new_element = new Node(data, tail);
			tail->next = new_element;
			tail = new_element;
		}
	}

	void pop()
	{
		if (!head)
			throw logic_error("Trying to remove from an empty container");
		else
		{
			if (head == tail)
			{
				delete tail;
				head = nullptr;
				tail = nullptr;
			}
			else
			{
				Node* temp = head->next;
				temp->pred = nullptr;
				delete head;
				head = temp;
			}
		}
	}

	void erase(T data) override
	{
		Node* temp = search(data);
		if (!temp)
			throw logic_error("No data to delete");
		else if (temp == head)
		{
			head = temp->next;
			delete temp;
		}
		else if (temp == tail)
		{
			tail = temp->pred;
			delete temp;
		}
		else
		{
			Node* p1 = temp->pred;
			Node* p2 = temp->next;
			p1->next = p2;
			p2->pred = p1;
			delete temp;
		}
	}
};
