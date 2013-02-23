/**
 * @file: asrt.h 
 * Definition of memory manager class. Utils library in ShowGraph.
 */
/*
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef MEM_MGR_H
#define MEM_MGR_H

namespace MemImpl
{
    /**
     * Event id type
     * @ingroup MemImpl
     */
    typedef quint64 MemEventId;

    /**
     * Memory manager implementation
     * @ingroup MemImpl
     */
    class MemInfo
    {
    public:
        /** Registers the allocation event and returns its number */
        MemEventId allocEvent();
        /** Registers the deallocation event and returns its number */
        MemEventId deallocEvent();
    private:
        
        /** Registers the allocation event - place for breakpoints */
        void allocReg( MemEventId n);
        /** Registers the deallocation event - place for breakpoints */
        void deallocReg( MemEventId n);
        
        /** Counter for allocation events */
        MemEventId alloc_counter;
        /** Counter for deallocation event */
        MemEventId dealloc_counter;
        /** private constructors, assignment and destructor */
        MemInfo();
        MemInfo( const MemInfo&){};
        MemInfo& operator =( const MemInfo&){};
        ~MemInfo(){};
        /** Needed for singleton creation */
        friend class Single< MemImpl::MemInfo>;
    };
};
#endif /* MEM_MGR_H */
