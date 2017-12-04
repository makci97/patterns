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
#include <type_traits>
#include "Typelist.h"
#include "Decompressors.h"

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

//template <typename AtomicType>
//class Sizeof {
//public:
//    static const size_t size = sizeof(AtomicType);
//};
//
//template <typename T1, typename T2>
//class Sizeof<Typelist<T1, T2>> {
//public:
//    static const size_t size = Sizeof<T1>::size + Sizeof<T2>::size;
//};
//
//template <>
//class Sizeof<NullType> {
//public:
//    static const size_t size = 0;
//};

template<bool B, class T, class AtomicType3, class InputTypeForAtomicType3>
struct ConditionalForOutputType { typedef T type; };

template<class T, class AtomicType3, class InputTypeForAtomicType3>
struct ConditionalForOutputType<false, T, AtomicType3, InputTypeForAtomicType3>
{
    typedef typename std::result_of<decltype(&AtomicType3::decompress)(InputTypeForAtomicType3)>::type type;
};

template <typename AtomicType1, typename AtomicType2, typename AtomicType3>
class OutputLineSizeof {
public:
    // if T2 not declared, that T3 take T1 as arg_type,
    // else T3 take result of T2.decompress()
    typedef typename std::conditional<
        std::is_same<AtomicType2, NoneType>::value,
        AtomicType1,
        typename std::result_of<decltype(&AtomicType2::decompress)(AtomicType2)>::type
    >::type InputTypeForAtomicType3;
    
    // if T3 not declared, that output_type == InputTypeForAtomicType3,
    // else output_type == result of T3.decompress(InputTypeForAtomicType3)
    typedef typename ConditionalForOutputType<
        std::is_same<AtomicType3, NoDecompressor>::value,
        InputTypeForAtomicType3,
        AtomicType3,
        InputTypeForAtomicType3
    >::type OutputType;
    
    static const size_t size = sizeof(OutputType);
};

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class OutputLineSizeof<Typelist<T1, T2>, Typelist<T3, T4>, Typelist<T5, T6>> {
public:
    static const size_t size = OutputLineSizeof<T1, T3, T5>::size + OutputLineSizeof<T2, T4, T6>::size;
};

template <>
class OutputLineSizeof<NullType, NullType, NullType> {
public:
    static const size_t size = 0;
};

template<bool B, class AtomicType1, class AtomicType2, class InputTypeForAtomicType3>
struct GetInputForType3 {
    static InputTypeForAtomicType3 get(AtomicType1 compressed)
    {
        return compressed;
    }
};

template<class AtomicType1, class AtomicType2, class InputTypeForAtomicType3>
struct GetInputForType3<false, AtomicType1, AtomicType2, InputTypeForAtomicType3>
{
    static InputTypeForAtomicType3 get(AtomicType1 compressed)
    {
        AtomicType2 input_2 = static_cast<AtomicType2>(compressed);
        return input_2.decompress();
    }
};

template<bool B, class AtomicType3, class InputTypeForAtomicType3>
struct GetOutput {
    static InputTypeForAtomicType3 get(InputTypeForAtomicType3 input_3)
    {
        return input_3;
    }
};

template<class AtomicType3, class InputTypeForAtomicType3>
struct GetOutput<false, AtomicType3, InputTypeForAtomicType3>
{
    static InputTypeForAtomicType3 get(InputTypeForAtomicType3 input_3)
    {
        return AtomicType3::decompress(input_3);
    }
};

template <typename AtomicType1, typename AtomicType2, typename AtomicType3>
class AddItem {
public:
    // if T2 not declared, that T3 take T1 as arg_type,
    // else T3 take result of T2.decompress()
    typedef typename std::conditional<
        std::is_same<AtomicType2, NoneType>::value,
        AtomicType1,
        typename std::result_of<decltype(&AtomicType2::decompress)(AtomicType2)>::type
    >::type InputTypeForAtomicType3;
    
    // if T3 not declared, that output_type == InputTypeForAtomicType3,
    // else output_type == result of T3.decompress(InputTypeForAtomicType3)
    typedef typename ConditionalForOutputType<
        std::is_same<AtomicType3, NoDecompressor>::value,
        InputTypeForAtomicType3,
        AtomicType3,
        InputTypeForAtomicType3
    >::type OutputType;
    
    static void* get_res_line(void* res, std::istringstream& is)
    {
        // read compressed data
        AtomicType1 compressed;
        is >> compressed;
        
        // decompress in AtomicType2
        InputTypeForAtomicType3 input_3 = GetInputForType3<
            std::is_same<AtomicType2, NoneType>::value,
            AtomicType1,
            AtomicType2,
            InputTypeForAtomicType3
        >::get(compressed);
        
        // decompress in AtomicType3
        OutputType res_value = GetOutput<
            std::is_same<AtomicType3, NoDecompressor>::value,
            AtomicType3,
            InputTypeForAtomicType3
        >::get(input_3);
        
        // write data to void*
        *(OutputType*)res = res_value;
        res = (void*)((OutputType*)res + 1);
        
        return res;
    }
};

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class AddItem<Typelist<T1, T2>, Typelist<T3, T4>, Typelist<T5, T6>> {
public:
    static void* get_res_line(void* res, std::istringstream& is)
    {
        res = AddItem<T1, T3, T5>::get_res_line(res, is);
        res = AddItem<T2, T4, T6>::get_res_line(res, is);
        return res;
    }
};

template <>
class AddItem<NullType, NullType, NullType> {
public:
    static void* get_res_line(void* res, std::istringstream& is)
    {
        return res;
    }
};

template <typename AtomicType1, typename AtomicType2, typename AtomicType3>
class PrintItem {
public:
    // if T2 not declared, that T3 take T1 as arg_type,
    // else T3 take result of T2.decompress()
    typedef typename std::conditional<
        std::is_same<AtomicType2, NoneType>::value,
        AtomicType1,
        typename std::result_of<decltype(&AtomicType2::decompress)(AtomicType2)>::type
    >::type InputTypeForAtomicType3;
    
    // if T3 not declared, that output_type == InputTypeForAtomicType3,
    // else output_type == result of T3.decompress(InputTypeForAtomicType3)
    typedef typename ConditionalForOutputType<
        std::is_same<AtomicType3, NoDecompressor>::value,
        InputTypeForAtomicType3,
        AtomicType3,
        InputTypeForAtomicType3
    >::type OutputType;
    
    static void* print(void* line)
    {
        OutputType elem = *(OutputType*)line;
        line = (void*)((OutputType*)line + 1);
        std::cout << elem << ' ';
        
        return line;
    }
};

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class PrintItem<Typelist<T1, T2>, Typelist<T3, T4>, Typelist<T5, T6>> {
public:
    static void* print(void* line)
    {
        line = PrintItem<T1, T3, T5>::print(line);
        line = PrintItem<T2, T4, T6>::print(line);
        return line;
    }
};

template <>
class PrintItem<NullType, NullType, NullType> {
public:
    static void* print(void* line)
    {
        return line;
    }
};

template <
    typename TLInputTypes,
    typename TLCompressedTypes,
    typename TLFinalDecompressorsTypes
>
class Reader {
public:
    Reader(std::string file_name) :
        _in(file_name)
    {
        _output_line_size = OutputLineSizeof<
            TLInputTypes,
            TLCompressedTypes,
            TLFinalDecompressorsTypes
        >::size;
    }

    void* get_next_line()
    {
        void* res = (void*)malloc(_output_line_size);
        std::string line;
        if (std::getline(_in, line))
        {
            std::istringstream is(line);
            res = AddItem<TLInputTypes, TLCompressedTypes, TLFinalDecompressorsTypes>::get_res_line(res, is);
            res = (void*)((char*)res - _output_line_size);
        }
        else
        {
            free(res);
            res = NULL;
        }
        return res;
        
        return res;
    }
    
    void print_line(void* line) const
    {
        PrintItem<
            TLInputTypes,
            TLCompressedTypes,
            TLFinalDecompressorsTypes
        >::print(line);
        line = (void*)((char*)line - _output_line_size);
    }
private:
    std::ifstream _in;
    size_t _output_line_size;
};

int f()
{
    return 1;
}

int main(int argc, const char * argv[])
{
    Reader<
        MakeTypelist<short, int, float, double>::Result,
        MakeTypelist<IntCompressedToShort_2, NoneType, DoubleCompressedToFloat_2, NoneType>::Result,
        MakeTypelist<LongIntCompressedToInt_3, LongIntCompressedToInt_3, NoDecompressor, LongDoubleCompressedToDouble_3>::Result
    > reader("a.txt");
    // a.txt
    // 1 22 3.1 7325.567
    // 34 54 4.3 33.54432

    void* line;
    while((line = reader.get_next_line()) != NULL)
    {
        reader.print_line(line);
        std::cout << std::endl;
        free(line);
    }
    
    std::cout << OutputLineSizeof<
        MakeTypelist<short, int, float, double>::Result,
        MakeTypelist<IntCompressedToShort_2, NoneType, DoubleCompressedToFloat_2, NoneType>::Result,
        MakeTypelist<LongIntCompressedToInt_3, LongIntCompressedToInt_3, NoDecompressor, LongDoubleCompressedToDouble_3>::Result
    > ::size << std::endl;
    //output == <long int, long int, double, long double>
    std::cout << sizeof(long int) * 2 + sizeof(double) + sizeof(long double) << std::endl;
    
    typedef std::result_of<decltype(&DoubleCompressedToFloat_2::decompress)(DoubleCompressedToFloat_2)>::type g;
    std::cout << sizeof(g);
    static_assert(std::is_same<g, double>::value, "bad");
    
//    std::cout << Sizeof<MakeTypelist<int, int, double>::Result>::size << std::endl;
//
//    Typelist<int, int>::Head i = 5;
//    Append<MakeTypelist<int>::Result, double>::Result::Tail::Head j = 6.1;
//    std::cout << i << " " << j << std::endl;
//    return 0;
}

