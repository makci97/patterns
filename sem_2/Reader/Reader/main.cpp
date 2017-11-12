//
//  main.cpp
//  Reader
//
//  Created by Максим on 07.11.17.
//  Copyright © 2017 Максим. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Typelist.h"

using namespace Loki;
using namespace TL;
//class EmptyClass
//{};
//
//template<typename ...Args>
//struct TypeList
//{
//    typedef EmptyClass Head;
//    typedef EmptyClass Tail;
//};
//
//typedef TypeList<> EmptyTypeList;
//
//template<typename H, typename ...T>
//struct TypeList<H, T...>
//{
//    typedef H Head;
//    typedef TypeList<T...> Tail;
//};
//
//template<typename T, typename TL>
//struct Add
//{
//};
//
//template<typename T, typename ...Args>
//struct Add<T, TypeList<Args...>>
//{
//    typedef TypeList<Args..., T> type;
//};

template <typename AtomicType>
class Sizeof {
public:
    static const size_t size = sizeof(AtomicType);
};

template <typename T1, typename T2>
class Sizeof<Typelist<T1, T2>> {
public:
    static const size_t size = Sizeof<T1>::size + Sizeof<T2>::size;
};

template <>
class Sizeof<NullType> {
public:
    static const size_t size = 0;
};

template <typename AtomicType>
class AddItem {
public:
    static void* get_res_line(void* res, std::istringstream& is)
    {
        AtomicType cur_elem;
        is >> cur_elem;
        *(AtomicType*)res = cur_elem;
//        std::cout << *(AtomicType*)res << ' ';
        res = (void*)((AtomicType*)res + 1);
//        std::cout << *((AtomicType*)res - 1)<< ' ';
        return res;
    }
};

template <typename T1, typename T2>
class AddItem<Typelist<T1, T2>> {
public:
    static void* get_res_line(void* res, std::istringstream& is)
    {
        res = AddItem<T1>::get_res_line(res, is);
        res = AddItem<T2>::get_res_line(res, is);
        return res;
    }
};

template <>
class AddItem<NullType> {
public:
    static void* get_res_line(void* res, std::istringstream& is)
    {
        return res;
    }
};

template <typename TL>
class Reader {
public:
    Reader(std::string file_name) : _in(file_name)
    {
    }

    void* GetNextLine()
    {
        size_t size = Sizeof<TL>::size;
        void* res = (void*)malloc(size);
        std::string line;
        if (std::getline(_in, line))
        {
//            std::cout << line << std::endl;
            
            std::istringstream is(line);
            res = AddItem<TL>::get_res_line(res, is);
            res = (void*)((char*)res - size);
//            std::memcpy(res, line.c_str(), size);
        }
        else
        {
            free(res);
            res = NULL;
        }
        return res;
    }
private:
    std::ifstream _in;
};

int main(int argc, const char * argv[])
{
    Reader<MakeTypelist<int, int, double>::Result> reader("a.txt");
    // a.txt
    // 1 22 3.1
    // 34 54 4.3

    void* line;
    while((line = reader.GetNextLine()) != NULL)
    {
        int a = *(int*)line;
        line = (void*)((int*)line + 1);
        int b = *(int*)line;
        line = (void*)((int*)line + 1);
        double c = *(double*)line;
        std::cout << a << ' ' << b << ' ' << c <<std::endl;
    }
    
    std::cout << Sizeof<MakeTypelist<int, int, double>::Result>::size << std::endl;
    
//    Typelist<int, int>::Head i = 5;
//    Append<MakeTypelist<int>::Result, double>::Result::Tail::Head j = 6.1;
//    std::cout << i << " " << j << std::endl;
//    return 0;
}

