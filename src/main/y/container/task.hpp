
//! \file
#ifndef Y_CONTAINER_TASK_INCLUDED
#define Y_CONTAINER_TASK_INCLUDED 1

#include "y/container/sequence.hpp"

namespace upsylon
{
    
    struct container_task
    {
        
        template <typename TARGET, typename ITERATOR> static inline
        void push_back(TARGET &target, ITERATOR curr, const ITERATOR last)
        {
            size_t done = 0;
            try {
                while(curr!=last)
                {
                    target.push_back( *(curr++) );
                    ++done;
                }
            }
            catch(...)
            {
                while( done-- > 0 )
                {
                    target.pop_back();
                }
                throw;
            }
        }
        
        template <typename TARGET, typename ITERATOR> static inline
        void push_back_(TARGET &target, ITERATOR curr, const ITERATOR last)
        {
            size_t done = 0;
            try {
                while(curr!=last)
                {
                    target.push_back_( *(curr++) );
                    ++done;
                }
            }
            catch(...)
            {
                while( done-- > 0 )
                {
                    target.pop_back();
                }
                throw;
            }
        }
        
        template <typename TARGET, typename ITERATOR> static inline
        void push_front(TARGET &target, ITERATOR curr, const ITERATOR last)
        {
            size_t done = 0;
            try {
                while(curr!=last)
                {
                    target.push_front( *(curr++) );
                    ++done;
                }
            }
            catch(...)
            {
                while( done-- > 0 )
                {
                    target.pop_front();
                }
                throw;
            }
        }
        
        
        
    };
}

#endif

