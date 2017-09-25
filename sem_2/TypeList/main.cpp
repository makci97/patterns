//
//  main.cpp
//  type_list
//
//  Created by Максим on 19.09.17.
//  Copyright © 2017 Максим. All rights reserved.
//

#include <iostream>


class EmptyClass
{};

template<typename ...Args>
struct TypeList
{
    typedef EmptyClass Head;
    typedef EmptyClass Tail;
};

typedef TypeList<> EmptyTypeList;

template<typename H, typename ...T>
struct TypeList<H, T...>
{
    typedef H Head;
    typedef TypeList<T...> Tail;
};

template<typename T, typename TL>
struct Add
{
};

template<typename T, typename ...Args>
struct Add<T, TypeList<Args...>>
{
    typedef TypeList<Args..., T> type;
};

int main(int argc, const char * argv[])
{
    TypeList<int, int>::Head i = 5;
    Add<double, TypeList<int>>::type::Tail::Head j = 6.1;
    std::cout << i << " " << j << std::endl;
    return 0;
}
