/**
 * @file: asrt.h 
 * Definition of memory manager class. Utils library in ShowGraph.
 */
/*
 * Copyright (c) 2009, Boris Shurygin
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
