//
//  Decompressors.h
//  Reader
//
//  Created by Максим on 14.11.17.
//  Copyright © 2017 Максим. All rights reserved.
//

#ifndef Decompressors_h
#define Decompressors_h

class DoubleCompressedToFloat_2 {
public:
    DoubleCompressedToFloat_2()
    { }
    
    DoubleCompressedToFloat_2(float f): _data(f)
    { }
    
    double decompress()
    {
        return (double)(_data+3.);
    }
private:
    float _data;
};

class IntCompressedToShort_2 {
public:
    IntCompressedToShort_2()
    { }
    
    IntCompressedToShort_2(short s): _data(s)
    { }
    
    int decompress()
    {
        return (int)(_data+2);
    }
private:
    short _data;
};

class LongDoubleCompressedToDouble_3 {
public:
    static long double decompress(double d)
    {
        return (long double)(d+3.);
    }
};

class LongIntCompressedToInt_3 {
public:
    static long int decompress(int i)
    {
        return (long int)(i+2);
    }
};

class NoneType
{
public:
    void decompress()
    { }
};

class NoDecompressor
{
    
};
#endif /* Decompressors_h */
