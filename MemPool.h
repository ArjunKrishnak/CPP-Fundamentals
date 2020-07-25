//  MemPool.h
//  Cpp-Fundamentals

#ifndef MemPool_h
#define MemPool_h

class MemPool
{
private:
    // The purpose of the structure`s definition is that we can operate linkedlist conveniently
    struct _Unit //The type of the node of linkedlist.
    {
        struct _Unit *pPrev, *pNext;
    };

    void* m_pMemBlock;                //The address of memory pool.

    // Manage all unit with two linkedlist.
    struct _Unit*    m_pAllocatedMemBlock; //Head pointer to Allocated linkedlist.
    struct _Unit*    m_pFreeMemBlock;      //Head pointer to Free linkedlist.

    unsigned long    m_ulUnitSize; //Memory unit size. There are much unit in memory pool.
    unsigned long    m_ulBlockSize; //Memory pool size. Memory pool is make of memory unit.

public:
    
    MemPool(unsigned long ulUnitNum ,
             unsigned long ulUnitSize /* Recommended 2**n */):
            m_pMemBlock(nullptr),
            m_pAllocatedMemBlock(nullptr),
            m_pFreeMemBlock(nullptr),
            m_ulBlockSize(ulUnitNum * (ulUnitSize+sizeof(struct _Unit))),
            m_ulUnitSize(ulUnitSize)
    {
        m_pMemBlock = malloc(m_ulBlockSize); //Allocate a memory block.
        if(m_pMemBlock)
        {
            for(unsigned long i=0; i<ulUnitNum; i++)  //Link all mem unit . Create linked list.
            {
                struct _Unit *pCurUnit = (struct _Unit *)( (char *)m_pMemBlock + i*(ulUnitSize+sizeof(struct _Unit)) );
                
                pCurUnit->pPrev = nullptr;
                pCurUnit->pNext = m_pFreeMemBlock;    //Insert the new unit at head.
                
                if(nullptr != m_pFreeMemBlock)
                {
                    m_pFreeMemBlock->pPrev = pCurUnit;
                }
                m_pFreeMemBlock = pCurUnit;
            }
        }
    }
    
    ~MemPool()
    {
        free(m_pMemBlock);
    }
    
     // Allocate memory unit If memory pool can`t provide proper memory unit,
     // It will call system function.
    void* Alloc(unsigned long ulSize, bool bUseMemPool = true)
    {
        if( ulSize > m_ulUnitSize || false == bUseMemPool ||
            nullptr == m_pMemBlock   || nullptr == m_pFreeMemBlock)
        {
            return malloc(ulSize);
        }

        // Now FreeList isn`t empty
        struct _Unit *pCurUnit = m_pFreeMemBlock;
        m_pFreeMemBlock = pCurUnit->pNext;  //Get a unit from free linkedlist.
        if(nullptr != m_pFreeMemBlock)
        {
            m_pFreeMemBlock->pPrev = nullptr;
        }

        pCurUnit->pNext = m_pAllocatedMemBlock;
        
        if(nullptr != m_pAllocatedMemBlock)
        {
            m_pAllocatedMemBlock->pPrev = pCurUnit;
        }
        m_pAllocatedMemBlock = pCurUnit;

        return (void *)((char *)pCurUnit + sizeof(struct _Unit) );
    }
    
    //  To free a memory unit. If the pointer of parameter point to a memory unit,
    //  then insert it to "Free linked list". Otherwise, call system function "free".
    void Free( void* p )
    {
       if(m_pMemBlock<p && p<(void *)((char *)m_pMemBlock + m_ulBlockSize) )
       {
           struct _Unit *pCurUnit = (struct _Unit *)((char *)p - sizeof(struct _Unit) );
           
           struct _Unit* pPrev = pCurUnit->pPrev;
           struct _Unit* pNext = pCurUnit->pNext;
           if( pPrev == nullptr){
               m_pAllocatedMemBlock = pCurUnit->pNext;
           }
           else{
               pPrev->pNext = pNext;
           }
           if(nullptr != pNext)
           {
               pNext->pPrev = pPrev;
           }

           pCurUnit->pNext = m_pFreeMemBlock;
           if(nullptr != m_pFreeMemBlock)
           {
                m_pFreeMemBlock->pPrev = pCurUnit;
           }

           m_pFreeMemBlock = pCurUnit;
       }
       else
       {
           free(p);
       }
    }
    
};




#endif /* MemPool_h */
