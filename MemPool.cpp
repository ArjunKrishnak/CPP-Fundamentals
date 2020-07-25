//  main.cpp
//  Cpp-Fundamentals

#include <iostream>
#include <chrono>
#include "MemPool.h"
using namespace std;

class Data
{
    char m_data[4096];
};

void NoPool()
{
    auto start = chrono::steady_clock::now();
    for(unsigned int i=0; i < 1000000; i++)
    {
        Data *p = new Data;
        delete p;
    }
    auto end = chrono::steady_clock::now();
    cout << "Elapsed Time NoPool  = " <<
        chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;
}

char buf[4096];  //Simple Memory Pool
class SharedData
{
public:
    void *operator new(size_t uiSize)
    {
        return (void *)buf;
    }
    void  operator delete(void *p)
    {
    }
};

void SimpleMemPool()
{
    auto start = chrono::steady_clock::now();
    for(unsigned int i=0; i < 1000000; i++)
    {
        SharedData *p = new SharedData;
        delete p;
    }
    auto end = chrono::steady_clock::now();
    cout << "Elapsed Time MemPool  = " <<
        chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;
}


int main(int argc, const char * argv[]) {
    cout << "Performance Test" << endl;
    cout << "Without Pool" << endl;
    NoPool();
    cout << "With Pool" << endl;
    SimpleMemPool();
    
    unsigned long ulUnitNum = 4;
    unsigned long ulUnitSize = 32;
    MemPool* pool = new MemPool(ulUnitNum,ulUnitSize);
    
    void* p1 = pool->Alloc(ulUnitSize);
    pool->Free(p1);
    
    delete pool; 

    return 0;
}
