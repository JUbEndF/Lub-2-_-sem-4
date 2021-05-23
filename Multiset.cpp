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
public:
	Tree_element(T key, Tree_element* prev = nullptr, Tree_element* left = nullptr, Tree_element* right = nullptr) : tkey(key), prev(prev), right(right), left(left) {};

	~Tree_element()
	{
		if (this->left != nullptr) delete left;
		if (this->right != nullptr) delete right;
	};
};




template <typename T>
class Multiset_tree
{
private:
	Tree_element<T>* root;

public:

	class Iterator
	{
	private:
		Tree_element<T>* iter;
	public:
		friend class Multiset_tree;
		friend class Tree_element<T>;
		Iterator(Tree_element<T>* current) : iter(current) {};

		T& operator*() const
		{
			return iter->tkey;
		}

		Tree_element<T>& operator->() const
		{
			return iter;
		}

		Iterator& operator++()
		{
			if (iter == nullptr)return this;
			if (iter.left != nullptr)
			{
				iter = iter.left;
				return this;
			}
			if (iter->right != nullptr)
			{
				iter = iter.right;
				return this;
			}
			else
			{
				Tree_element* save = iter;
				while (iter.prev != nullptr && ((iter.prev).right == nullptr || (iter.prev).right == iter))
				{
					iter = iter.prev;
				}
				if (iter.prev == nullptr)
				{
					iter = save;
					return this;
				}
				iter = (iter.prev).right;
				return this;
			}
		}

		Iterator& operator--()
		{
			if (iter == nullptr) return this;
			if (iter == (iter->prev)->right)
			{
				if (iter->prev->left != nullptr)
				{
					iter = (iter->prev)->left;
					return this;
				}
				else
				{
					iter = iter->prev;
					return this;
				}
			}
			if (iter == (iter->prev)->left && iter->prev != nullptr)
			{
				iter = iter->prev;
				return this;
			}
			else
			{
				return this;
			}
		}

		~Iterator() {};
	};

	Tree_element<T>* create_element(T key, Tree_element<T>* prev = nullptr)
	{
		return new Tree_element<T>(key, prev);
	}

	Multiset_tree(int n, T...)// Tree(4, 1,2,3,4);
	{
		if (n == 0)
		{
			root = nullptr;
			return;
		}
		va_list factor;
		va_start(factor, n);
		T k = va_arg(factor, T);
		root = new Tree_element<T>(k, nullptr, nullptr, nullptr);
		Iterator create(root);
		if (n>1)
		{
			for (int i = 1; i < n; i++)
			{
				k = va_arg(factor, T);
				insert(k);
			}
		}
	};

	Multiset_tree(T*array, int size)
	{
		if (array == nullptr)
		{
			root = nullptr;
			return;
		}
		for (size_t i = 0; i < size; i++)
		{
			insert(array[i]);
		}
	}

	Iterator insert(T k)
	{
		Tree_element<T>* current = root;
		if (root == nullptr)
		{
			root = new Tree_element<T>(k);
			return { root };
		}
		while (current)
		{
			if (current->tkey < k)
			{
				if (current->right == nullptr) 
				{ 
					current->right = create_element(k, current);
					return { current->right };
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
						return { current->left };
					}
					else current = current->left;
				}
				else
				{
					Tree_element<T>* cur_left = current->left;
					Tree_element<T>* created = new Tree_element<T>(k, current, cur_left);
					cur_left->prev = created;
					return { created };
				}
			}
		}
	}

	Iterator erase(Iterator del)
	{
		Tree_element<T>* delet = del.iter;
		if (delet->left == delet->right == nullptr)
		{
			if (delet == delet->prev->left) delet->prev->left = nullptr;
			if (delet == delet->prev->right) delet->prev->right = nullptr;
			delete delet;
		}
		if (delet->right == nullptr)
		{
			delet->left->prev = delet->prev;
			if (delet == delet->prev->left) delet->prev->left = delet->left;
			else delet->prev->right = delet->left;
			delet->left = nullptr;
			delet->right = nullptr;
			delete delet;
		}
		else
		{
			Tree_element<T>* tmp = delet->right;
			while (tmp->left != nullptr)
			{
				tmp = tmp->left;
			}
			delet->tkey = tmp->tkey;
			erase(Iterator(tmp));
			return { delet };
		}
	}

	Iterator begin()
	{
		return { root };
	}

	Iterator end()
	{
		Iterator tmp(root);
		while (tmp != tmp++)
		{

		}
		return tmp;
	}

	Iterator find(T k) const
	{
		Tree_element<T>* current = root;
		while (current != nullptr)
		{
			if (current->tkey == k)return Iterator(current);
			if (current->tkey < k)current = current->right;
			if (current->tkey > k)current = current->left;
		}
		return Iterator<T>(current);
	}

	size_t count(T k) const
	{
		size_t coun = 0;
		Iterator tmp(root);
		do
		{
			if (tmp.iter->tkey == k)coun++;
		} while (tmp != tmp++);
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