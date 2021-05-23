#include <iostream>


int default_size = 10;

template <typename T>
class Data
{
	T value;
	Data* prev;
	Data* next;
	template <typename T>
	friend class Bucket;
	template <typename T>
	friend class Hash_table;
	template <typename T>
	friend class Iterator_h;
public:
	Data(T v, Data* n = nullptr, Data*p=nullptr) : value(v), next(n), prev(p)
	{

	}

	~Data()
	{
		if (next != nullptr)
		{
			delete next;
		}
	}
};

template <typename T>
class Bucket
{
private:
	Data<T>* first;
	template <typename T>
	friend class Data;
	friend class Hash_table<T>;
	friend class Iterator_h<T>;
public:
	Bucket(Data<T>* create):first(create) {}

	Bucket()
	{
		first = nullptr;
	}

	Data<T>* insert(const T& value)
	{
		if (first == nullptr)
		{
			first = new Data<T>(value);
			return first;
		}
		Data<T>* i = first;
		while (i->value < value && i->next != nullptr)
		{
			i = i->next;
		}
		if (i->value >= value)
		{
			if (i->prev == nullptr)
			{
				first = new Data<T>(value,i);
				i->prev = first;
				return first;
			}
			else
			{
				Data<T>* prev_create = i->prev;
				prev_create->next = new Data<T>(value, i, i->prev);
				i->prev = prev_create->next;
				return i->prev;
			}
		}
		else
		{
			i->next = new Data<T>(value, nullptr, i);
			return i->next;
		}
		
	}

	Data<T>* end()
	{
		if (first == nullptr)
		{
			return nullptr;
		}
		Data<T>* ret = this->first;
		for (ret; ret->next != nullptr; ret = ret->next)
		{

		}
		return ret;
	}

	Data<T>* begin()
	{
		if (first == nullptr)
		{
			return nullptr;
		}
		return this->first;
	}

	Data<T>* erase(Data<T>* del)
	{
		if (del->next == nullptr)
		{
			if (del->prev == nullptr)
			{
				delete this;
				return nullptr;
			}
			del->prev->next = nullptr;
			Data<T>* ret = del->prev;
			delete del;
			return ret;
		}
		else
		{
			if (del->prev == nullptr)
			{
				Data<T>* save = first->next;
				first->next = nullptr;
				delete first;
				first = save;
				first->prev = nullptr;
				return first;
			}
			Data<T>* r = del->prev;
			r->next = del->next;
			del->next->prev = r;
			del->next = nullptr;
			del->prev = nullptr;
			delete del;
			return r;
		}
	}

	int count(const T& value)
	{
		Data<T>* i = first;
		int cout = 0;
		for (i; i != nullptr && i->value != value; i = i->next) {}
		if (i == nullptr)return cout;
		else
		{
			while (i->next != nullptr && i->value == value)
			{
				i = i->next;
				cout++;
			}
			if(i->next == nullptr)
				cout++;
			return cout;
		}
	}

	~Bucket()
	{
		if (first != nullptr)
			delete first;
	}

};


template <typename T>
class Iterator_h
{
private:
	Hash_table<T>* object;
	Bucket<T>* current_bucket;
	Data<T>* current_element;
	int index_iterator;

	friend class Hash_table<T>;

public:
	Iterator_h(Hash_table<T>* tmp) : object(tmp)
	{
		if (tmp->real_size == 0)
		{
			current_bucket = nullptr;
			current_element = nullptr;
			index_iterator = -1;
		}
		else
		{
			current_bucket = object->table[0];
			current_element = current_bucket->first;
			index_iterator = 0;
		}
	}

	Iterator_h(Hash_table<T>* tmp, int index_bucket, Data<T>* element) : object(tmp), current_bucket(tmp->table[index_bucket]), current_element(element), index_iterator(index_bucket)
	{

	}

	Iterator_h()
	{
		object = nullptr;
		current_bucket = nullptr;
		current_element = nullptr;
		index_iterator = -1;
	}

	T& operator*()
	{
		if (current_element == nullptr)throw std::exception("The iterator is empty");
		return current_element->value;
	}

	T* operator->()
	{
		if (current_element == nullptr)throw std::exception("The iterator is empty");
		return &(current_element->value);
	}

	Hash_table<T>* point_object()
	{
		return object;
	}

	Bucket<T>* pointer_bucket()
	{
		return current_bucket;
	}

	Iterator_h& operator++()
	{
		if (object != nullptr)
		{
			if (current_element->next != nullptr)
			{
				current_element = current_element->next;
				return *this;
			}
			if (index_iterator != (object->buffer_size))
			{
				int i = index_iterator;
				i++;
				for(i; i < object->buffer_size && (*object).table[i] == nullptr; i++) {}

				if(i == object->buffer_size || (*object).table[i] == nullptr) return *this;
				current_bucket = object->table[i];
				index_iterator = i;
				current_element = current_bucket->first;
				return *this;
			}
		}
		return *this;
	}

	Iterator_h& operator--()
	{
		if (index_iterator == 0 && current_element->prev == nullptr)
		{
			return *this;
		}
		if (current_element->prev != nullptr)
		{
			current_element = current_element->prev;
			return *this;
		}
		else
		{
			if (index_iterator != 0)
			{
				index_iterator--;
				current_bucket = object->table[index_iterator];
				Data<T>* i = current_bucket->first;
				for (i; i->next != nullptr; i = i->next)
				{
				}
				current_element = i;
			}
			return *this;
		}
	}
};

template <typename T>
class Hash_table
{
private:
	

	template <typename T>
	friend class Bucket;
	friend class Data<T>;
	template <typename T>
	friend class Iterator_h;

	int buffer_size = default_size;
	Bucket<T>** table;
	int real_size;
	int number_element;


public:
	Hash_table()
	{
		real_size = 0;
		number_element = 0;
		table = new Bucket<T> *[buffer_size];
		for (int i = 0; i < buffer_size; i++)
		{
			table[i] = nullptr;
		}
	}

	int ret_size() const
	{
		return number_element;
	}

	/*void Resize()
	{
		int last_buffer_size = buffer_size;
		buffer_size *= 2;
		Bucket<T>** new_array = new Bucket<T> * [buffer_size];
		for (int i = 0; i < buffer_size; i++)
		{
			new_array[i] = nullptr;
		}
		for (int i = 0; i < real_size; i++)
		{
			new_array[i] = table[i];
			table[i] = nullptr;
		}
		for (int i = 0; i < last_buffer_size; i++)
			swap(new_array[i], table[i]);
		delete[]new_array;
	}*/

	Iterator_h<T>& insert(const T& value)
	{
		int i = std::hash<T>()(value) % default_size;
		if (table[i] != nullptr)
		{
			Bucket<T>* create = table[i];
			Iterator_h<T> iter(this, i, create->insert(value));
			number_element++;
			return iter;
		}
		/*if (i == real_size && real_size == ) Resize();*/
		table[i] = new Bucket<T> ();
		Bucket<T>* create_2 = table[i];
		Iterator_h<T> iter_2(this, i, create_2->insert(value));
		number_element++;
		real_size++;
		return iter_2;
	}

	Iterator_h<T>& erase(Iterator_h<T> del)
	{
		if (del.current_element == nullptr || del.current_bucket == nullptr || del.object == nullptr) throw std::exception("Empty iterator can not be deleted");
		del.current_element = del.current_bucket->erase(del.current_element);
		number_element--;
		if (del.current_element == nullptr)
		{
			del.current_bucket = nullptr;
			table[del.index_iterator] = nullptr;
			real_size--;
			int i = del.index_iterator + 1;
			for (i; i < buffer_size; i++)
				if (table[i] != nullptr) {
					del.index_iterator = i;
					del.current_bucket = table[i];
					del.current_element = del.current_bucket->begin();
					return del;
				}
			i = del.index_iterator - 1;
			for (i; i >= 0; i--)
				if (table[i] != nullptr) {
					del.index_iterator = i;
					del.current_bucket = table[i];
					del.current_element = del.current_bucket->begin();
					return del;
				}
			Iterator_h<T>ret;
			return ret;
		}
	}

	Iterator_h<T>& find(const T& value)
	{
		int i = std::hash<T>()(value) % default_size;
		Iterator_h<T>ret;
		Bucket<T>* f = table[i];
		for (Data<T>* iter = f->first; iter != nullptr; iter = iter->next)
			if (iter->value == value)
			{
				Iterator_h<T>ret_2(this, i, iter);
				return ret_2;
			}
		return ret;
	}

	int count(T value)
	{
		Iterator_h<T> element = find(value);
		int ret = 0;
		if (element.current_element == nullptr)return 0;
		for (Data<T>* i = element.current_element; i != nullptr && i->value == value; i = i->next)
			ret++;
		return ret;
	}

	Iterator_h<T> begin()
	{
		int i = 0;
		while (i < buffer_size && table[i] == nullptr) { i++; }
		Bucket<T>* f = table[i];
		if (f == nullptr)return Iterator_h<T>();
		Iterator_h<T> ret(this, i, f->first);
		return ret;
	}

	Iterator_h<T>& end()
	{
		int i = 0;
		int j = 0;
		for (int y = 0; y < real_size; y++)
		{
			for (i; i < buffer_size && table[i] == nullptr; i++) {}
			if (table[i] != nullptr) { j = i; }
		}
		Iterator_h<T> ret;
		if (table[j] != nullptr)
		{
			Bucket<T>* f = table[j];
			ret = Iterator_h<T>(this, j, f->end());
		}
		return ret;
	}

	void swap(Bucket<T>* first, Bucket<T>* second)
	{
		if (first == nullptr || second == nullptr)
		{
			return;
		}
		Data<T>* save_2 = second->first;
		second->first = first->first;
		first->first = save_2;
	}

	/*void print()
	{
		Iterator_h<T> tmp(this->begin());
		for (int i = 0; i < number_element; i++, ++tmp)
			std::cout << *tmp << " ";
		std::cout << std::endl;
	}*/

	~Hash_table()
	{
		for (int i = 0; i < real_size; i++)
		{
			delete table[i];
		}
		delete []table;
	}
};


