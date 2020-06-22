//  main.hpp
//  Created by Arjun krishna
//  Copyright © 2020 Arjun krishna. All rights reserved.


#ifndef main_hpp
#define main_hpp

#include <string>
#include <iostream>
#include <memory>

void RawPointer()
{
//     Using a raw pointer -- not recommended.
    int* pint = new int(0);
//     You have to explicitly correctly delete any allocations to heap!
    delete pint;
}

namespace CustomSmartPointers{

    class UniquePointer
    {
        public:
            int* mPtr; // Actual pointer
            UniquePointer(int* p = nullptr): mPtr(p) {};
            UniquePointer(UniquePointer & up) = delete;
            UniquePointer& operator = (UniquePointer &up) = delete;
        
            UniquePointer(UniquePointer && up)
            {
                mPtr = up.mPtr;
                up.mPtr = nullptr;
            }
            UniquePointer& operator = (UniquePointer &&up)
            {
                if(this == &up)
                    return *this;
                mPtr = up.mPtr;
                up.mPtr = nullptr;
                return *this;
            }
            ~UniquePointer() { delete (mPtr); }
            int& operator*() { return *mPtr; }
    };
    
    class Counter
    {
        public:
            unsigned int mCount;
            Counter() : mCount(0){};
            void operator++()
            {
                mCount++;
            }
            void operator--()
            {
                mCount--;
            }
            ~Counter() = default;
            
    };
    

    class SharedPointer
    {
        public:
            CustomSmartPointers::Counter *mCount;
            int* mPtr;
        
            SharedPointer(int* ptr = nullptr): mPtr(ptr)
            {
                mCount = new Counter();
                if (ptr)
                {
                    mCount++;
                }
            }
        
            SharedPointer(SharedPointer & sp)
            {
                mPtr = sp.mPtr;
                mCount = sp.mCount;
                sp.mCount++;
            }
            
            SharedPointer(SharedPointer && sp)
            {
                mPtr = sp.mPtr;
                mCount = sp.mCount;
                sp.mPtr  = nullptr;
                sp.mCount = nullptr;
            }
        
            SharedPointer& operator = (SharedPointer &sp)
            {
                if(this == &sp)
                    return *this;
                mPtr = sp.mPtr;
                mCount = sp.mCount;
                sp.mCount++;
                return *this;
            }
            SharedPointer& operator = (SharedPointer &&sp)
            {
                if(this == &sp)
                    return *this;
                mPtr = sp.mPtr;
                mCount = sp.mCount;
                sp.mPtr  = nullptr;
                sp.mCount = nullptr;
                return *this;
            }
            
            ~SharedPointer()
            {
                if(mCount->mCount == 1){
                    delete (mPtr);
                    delete mCount;
                }
                mPtr = nullptr;
                
            }
        
            int& operator*() { return *mPtr; }
        
    };

    class WeakPointer
    {
        public:
            int* mPtr;
            WeakPointer(SharedPointer & sp) :mPtr(sp.mPtr)
            {
                // ..
            };
            WeakPointer(WeakPointer & sp) = default;
            WeakPointer(WeakPointer && wp) = delete;
            WeakPointer& operator = (WeakPointer &up) = default;
            WeakPointer& operator = (WeakPointer &&up) = delete;
            ~WeakPointer()
            {
                // donot delete mPtr
                mPtr = nullptr;
            }
            int& operator*()
            {
                // returned data maybe invalid
                return *mPtr;
            }
    };
}


void UniquePointer()
{
    /**
     * Allows exactly one owner of the underlying pointer.
     * unique_ptr can be moved, but not copied.
     * Moving transfers ownership to a new unique_ptr and resets the old unique_ptr.
     * auto_ptr was deprecated by unique ptr.
    */
    
//    Using new  -- not recommended.
//      std::unique_ptr<int> up1(new int(0)); // Not reccomended
    auto up1 = std::make_unique<int>(0);
    
//     Move raw pointer from one unique_ptr to another via assignment.
    auto up2 = std::move(up1);
    
    up1 == nullptr ? std::cout<< " up1 == nullptr " << std::endl :
                     std::cout<< " up1 != nullptr " << std::endl;
    
//     Move raw pointer from one unique_ptr to another via copy constructor.
    auto up3(std::move(up2));

//  Cannot create copies of pointer using copy ctor ot assignment operator
//    std::unique_ptr<int> up4(up3);
//    std::unique_ptr<int> up5 = up3;
    
//     Don't need to delete!
}

void SharedPointer()
{
    /* Reference-counted smart pointer. */
    std::shared_ptr<int>  sp1 = std::make_shared<int>(0);
    auto sp2 = sp1;
    auto sp3(sp2);
    auto sp4 = std::move(sp3);
    auto sp5(std::move(sp4));
    
    std::cout<<"sp1.use_count() = "<< sp1.use_count() << std::endl;
    
    
    // Create two unrelated shared_ptrs.
    auto sp6 = std::make_shared<int>(0);
    auto sp7 = std::make_shared<int>(0);

    sp6 == sp7 ? std::cout << " sp6 == sp7 " << std::endl :
                 std::cout << " sp6 != sp7" << std::endl;
    sp1 == sp5 ? std::cout << " sp1 == sp5 " << std::endl :
                 std::cout << " sp1 != sp5" << std::endl;
    
    sp6.reset();
    

    /*  Multiple threads can read and write different shared_ptr ints at the same time, even
        when the ints are copies that share ownership. */
    
//     Don't need to delete!
}

void WeakPointer()
{
    /*
       A weak_ptr provide a way to observe data that is owned by one
       or more shared_ptr instances, but does not participate in reference counting
     */
    
    std::shared_ptr<int>  sp1 = std::make_shared<int>(5);
    std::weak_ptr<int> wp1(sp1);
    std::cout<<"sp1.use_count() = "<< sp1.use_count() << std::endl;
    std::cout << "*wp1.lock() == " << *wp1.lock() << std::endl;
    
    sp1.reset();
    wp1.expired() ? std::cout << "wp.expired() == true" << std::endl :
                    std::cout << "wp.expired() == false" << std::endl;
    std::cout << "*wp1.lock() == " << *wp1.lock() << std::endl; // Bad access
}

int main(int argc, const char * argv[])
{
    
    UniquePointer();
    SharedPointer();
    WeakPointer();
}

#endif /* main_hpp */

