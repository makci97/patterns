//
//  main.cpp
//  GenLinearHierarchy
//
//  Created by Максим on 31.10.17.
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




/*
class TextBox
{
    
};

class Image
{
    
};

template <class T>
class Controller
{
};

template <>
class Controller<TextBox>
{
    // ....
};

template <>
class Controller<Image>
{
    // ....
};

class View : Controller<TextBox>, Controller<Image>
{
    
};
*/

template <class T,  class A>
class Controller: A
{
};


class RootType
{
    
};

template <class T>
class Controller <T, RootType> {
    
};

// Unit == Contorller
template <class T1, template<class, class> class Unit, class Root>
class GenLinearHierarchy {
    
};

template <class T1, class T2, template<class, class> class Unit, class Root>
class GenLinearHierarchy<TypeList<T1, T2>, Unit, Root> : Unit<T1, GenLinearHierarchy<T2, Unit, Root> >
{
};

template <class T, template<class, class> class Unit>
class GenLinearHierarchy<TypeList<T>, Unit, RootType>
: Unit<T, RootType>
{
};


int main(int argc, const char * argv[])
{
    TypeList<int, int>::Head i = 5;
    Add<double, TypeList<int>>::type::Tail::Head j = 6.1;
    std::cout << i << " " << j << std::endl;
    return 0;
}

