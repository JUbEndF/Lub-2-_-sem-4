#include <iostream>
#include "Multiset.h"
#include "Unordered_multiset.h"
#include <random>
#include <ctime>

template <typename T>
//using Set = Multiset_tree<T>;
using Set = Hash_table<T>;
template <typename T>
//using Iterator_set = Iterator<T>;
using Iterator_set = Iterator_h<T>;

template <typename T>
void filling_array(T* array, size_t size)
{
    srand((unsigned)time(NULL));
    for (size_t i = 0; i < size; i++)
    {
        array[i] = rand() % 5;
    }
}

template <typename T>
void print(Set<T>& object)
{
    Iterator_h<T> tmp = object.begin();
    for (int i = 0; i < object.ret_size(); i++, ++tmp)
        std::cout << *tmp << " ";
    std::cout << std::endl;
}

int main()
{ 
    /* std::random_device e;
    std::mt19937 gen(e());
    std::normal_distribution<float> d(0, 20);
    int size = (int)d(gen);
    float* data = new float[size];
    std::for_each(data, data+size, [&gen, &d](float& x) {x = d(gen); });


    for (int i = 0; i < size; i++)
        std::cout << " " << data[i];*/

    srand((unsigned)time(NULL));
    int j;
    do
    {
        j = rand() % 20;
    } while (j == 0);

    int* arr = new int[j];
    filling_array(arr, j);

    try
    {
        //Set<int> table(nullptr, 0);// для дерева
        Set<int> table;
        for (int i = 0; i < j; i++)
            table.insert(arr[i]);
        Iterator_set<int>tmp(table.begin());
        print(table);

        int user_value;
        std::cout << "Enter value to delete: ";
        std::cin >> user_value;
        tmp = table.find(user_value);
        table.erase(tmp);
        std::cout << std::endl;
        print(table);

        std::cout << "Enter value to delete: ";
        std::cin >> user_value;
        tmp = table.find(user_value);
        table.erase(tmp);
        std::cout << std::endl;
        print(table);\

        std::cout << "Enter value to add: ";
        std::cin >> user_value;
        table.insert(user_value);
        std::cout << std::endl;
        print(table);

        std::cout << "Enter value to find: ";
        std::cin >> user_value;
        Iterator_set<int>tmp_2(table.find(user_value));
        std::cout<<*tmp_2 << " element repeats: "<<table.count(*tmp_2);
        std::cout << std::endl;
        print(table);
        
        std::cout << "Enter value to add: ";
        std::cin >> user_value;
        table.insert(user_value);
        std::cout << std::endl;
        print(table);
        
        std::cout << "Enter value to delete: ";
        std::cin >> user_value;
        tmp = table.find(user_value);
        table.erase(tmp);
        std::cout << std::endl;
        print(table);
       
        std::cout << "Delete nullptr"<< std::endl;
        table.erase(table.end());
        std::cout << std::endl;
        print(table);
    }
    catch (std::exception Error)
    {
        std::cout << Error.what();
    }
}