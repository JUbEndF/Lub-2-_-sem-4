#include <iostream>
#include <cstdarg>

template <typename T>
class Tree_element
{
private:
	T tkey;
	Tree_element* prev;
	Tree_element* left = nullptr;
	Tree_element* right = nullptr;
	template <typename T>
	friend class Multiset_tree;
	template <typename T>
	friend class Iterator;

	Tree_element* min_left()
	{
		Tree_element* iter = this;
		while (iter->left != nullptr)
		{
			iter = iter->left;
		}
		return iter;
	}

	Tree_element* max_right()
	{
		Tree_element* iter = this;
		while (iter->right != nullptr)
		{
			iter = iter->right;
		}
		return iter;
	}

public:
	Tree_element(T key, Tree_element* prev = nullptr, Tree_element* left = nullptr, Tree_element* right = nullptr) : tkey(key), prev(prev), right(right), left(left) {};

	

	~Tree_element()
	{
		if (this->left != nullptr) delete left;
		if (this->right != nullptr) delete right;
	};
};

template <typename T>
class Iterator
{
private:
	Tree_element<T>* iter;
	Tree_element<T>* root;
public:
	template <typename T>
	friend class Multiset_tree;
	friend class Tree_element<T>;

	Iterator(Tree_element<T>& current) : iter(&current) 
	{
		root = iter;
		if (iter->prev == nullptr) iter = iter->min_left();
		while (root->prev != nullptr)
		{
			root = root->prev;
		}
	};

	Iterator(Iterator&& second) noexcept
	{
		this->iter = second.iter;
		this->root = second.root;
		second.iter = nullptr;
		second.root = nullptr;
	};

	Iterator() :iter(nullptr), root(nullptr) {};

	Iterator(const Iterator& current) : iter(current.iter), root(current.root) {};
	
	bool operator!=(const Iterator& second)const
	{
		if (second.iter != this->iter)return true;
		return false;
	}

	bool operator==(const Iterator& second)const
	{
		if (second.iter == this->iter)return true;
		return false;
	}

	void operator=(const Iterator& second)
	{
		this->iter = second.iter;
		this->root = second.root;
	}

	T& operator*() const
	{
		if (iter == nullptr)throw std::exception("The iterator is empty");
		return iter->tkey;
	}

	T* operator->() const
	{
		if (iter == nullptr)throw std::exception("The iterator is empty");
		return &(iter->tkey);
	}

	Iterator<T>& operator++()
	{
		if (iter == nullptr)return *this;
		if (iter->prev != nullptr)
		{
			if (iter->right != nullptr)
			{
				iter = iter->right;
				while (iter->left != nullptr)
					iter = iter->left;
				return *this;
			}
			if (iter->prev != nullptr && iter == iter->prev->left)
			{
				iter = iter->prev;
				return *this;
			}
			if (iter->prev != nullptr && iter == iter->prev->right)
			{
				Tree_element<T>* save = iter;
				while (iter->prev != nullptr && iter == iter->prev->right)
				{
					iter = iter->prev;
				}
				if (iter == root)
					iter = save;
				else
					iter = iter->prev;
				return *this;
			}
		}
		else
		{
			if(iter->right != nullptr)
				iter = iter->right->min_left();
			return *this;
		}
	}

	Iterator<T>& operator--()
	{
		if (iter == nullptr)return *this;
		if (iter->prev == nullptr)
		{
			if(iter->left != nullptr)
				iter = iter->left->max_right();
			return *this;
		}
		
		if (iter->left != nullptr)
		{
			iter = iter->left->max_right();
			return *this;
		}
		else
		{
			if (iter == iter->prev->right)
			{
				iter = iter->prev;
				return *this;
			}
			if (iter == iter->prev->left)
			{
				Tree_element<T>* save = iter;
				while (iter->prev != nullptr && iter == iter->prev->left)
				{
					iter = iter->prev;
				}
				if (iter == root)
					iter = save;
				else
					iter = iter->prev;
				return *this;
			}
		}
	}

	~Iterator() {};
};


template <typename T>
class Multiset_tree
{
private:
	Tree_element<T>* root;
	int size;
public:
	Tree_element<T>* create_element(T key, Tree_element<T>* prev = nullptr)
	{
		return new Tree_element<T>(key, prev);
	}

	int ret_size()
	{
		return size;
	}

	Multiset_tree(int n, T...)// Tree(4, 1,2,3,4);
	{
		size = 0;
		if (n == 0)
		{
			root = nullptr;
			return;
		}
		root = nullptr;
		va_list factor;
		va_start(factor, n);
		T k = va_arg(factor, T);
		if (n > 1)
		{
			for (int i = 1; i <= n; i++)
			{
				insert(k);
				k = va_arg(factor, T);
			}
		}
	};

	Multiset_tree(T* array, int size)
	{
		this->size = 0;
		if (array == nullptr)
		{
			root = nullptr;
			return;
		}
		for (int i = 0; i < size; i++)
		{
			insert(array[i]);
		}
	}

	Iterator<T> insert(const T& k)
	{
		Tree_element<T>* current = root;
		Iterator<T> ret;
		if (root == nullptr)
		{
			root = new Tree_element<T>(k);
			size++;
			ret.iter = root;
			return ret;
		}
		while (current)
		{
			if (current->tkey < k)
			{
				if (current->right == nullptr)
				{
					current->right = create_element(k, current);
					ret.iter = current->right;
					size++;
					return ret;
				}
				else current = current->right;
			}
			else
			{
				if (current->tkey > k)
				{
					if (current->left == nullptr)
					{
						current->left = create_element(k, current);
						ret.iter = current->left;
						size++;
						return ret; 
					}
					else current = current->left;
				}
				else
				{
					if (current->left == nullptr)
					{
						current->left = create_element(k, current);
						ret.iter = current->left;
						size++;
						return ret;
					}
					Tree_element<T>* cur_left = current->left;
					Tree_element<T>* created = new Tree_element<T>(k, current, cur_left);
					cur_left->prev = created;
					current->left = created;
					ret.iter = created;
					size++;
					return ret;
				}
			}
		}
	}

	Iterator<T> erase(Iterator<T> del)
	{
		if (del.iter == nullptr)
		{
			throw std::exception("Empty iterator can not be deleted");
		}
		Tree_element<T>* delet = del.iter;
		Iterator<T> ret;
		this->size--;
		if (delet->left ==nullptr && delet->right == nullptr)
		{
			ret.iter = delet->prev;
			if (delet->prev != nullptr)
			{
				if (delet == delet->prev->left) delet->prev->left = nullptr;
				if (delet == delet->prev->right) delet->prev->right = nullptr;
			}
			delete delet;
			return ret;
		}
		if (delet->right == nullptr)
		{
			delet->left->prev = delet->prev;
			if (delet->prev != nullptr)
			{
				if (delet == delet->prev->left) delet->prev->left = delet->left;
				else delet->prev->right = delet->left;
			}
			ret.iter = delet->left;
			delet->left = nullptr;
			delet->right = nullptr;
			delete delet;
			return ret;
		}
		else
		{
			if (delet->left == nullptr)
			{
				delet->right->prev = delet->prev;
				if (delet->prev != nullptr)
				{
					if (delet == delet->prev->left) delet->prev->left = delet->right;
					else delet->prev->right = delet->right;
				}
				ret.iter = delet->right;
				delet->left = nullptr;
				delet->right = nullptr;
				delete delet;
				return ret;
			}
			else
			{
				Tree_element<T>* tmp = delet->right;
				while (tmp->left != nullptr)
				{
					tmp = tmp->left;
				}
				delet->tkey = tmp->tkey;
				Iterator<T> e(*tmp);
				erase(e);
				this->size++;
				ret.iter = delet;
				return ret;
			}
			
		}
	}

	Iterator<T> begin()
	{
		Iterator<T> ret(*root->min_left());
		return ret;
	}

	Iterator<T> end()
	{
		Iterator<T> tmp(*root);
		for (int i = 0; i < size; i++)
			++tmp;
		tmp.iter = tmp.iter->right;
		return tmp;
	}

	Iterator<T> find(T k) const
	{
		Tree_element<T>* current = root;
		Iterator<T> ret(*current);
		while (current != nullptr)
		{
			if (current->tkey == k) { ret.iter = current; return ret; }
			if (current->tkey < k)current = current->right;
			else if (current->tkey > k)current = current->left;
		}
		return Iterator<T>();
	}

	void print()
	{
		Iterator<T>tmp(*root);
		for (int i = 0; i < size; i++, ++tmp)
			std::cout << *tmp << " ";
		std::cout << std::endl;
	}

	int count(T k)
	{
		int coun = 0;
		Iterator<T> tmp(*root->min_left());
		for(int i = 0; i< this->size; i++, ++tmp)
			if (tmp.iter->tkey == k)coun++;
		return coun;
	}

	~Multiset_tree()
	{
		if (root != nullptr)
		{
			delete root;
		}
	};
};
