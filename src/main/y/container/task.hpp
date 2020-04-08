
//! \file
#ifndef Y_CONTAINER_TASK_INCLUDED
#define Y_CONTAINER_TASK_INCLUDED 1

#include "y/container/sequence.hpp"

namespace upsylon
{
    
    //! common tasks for container
    struct container_task
    {
        
        //! push back a range to target
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
        
        //! push back a source into target
        template <typename TARGET, typename SOURCE> static inline
        void push_back(TARGET &target, SOURCE &source )
        {
            push_back(target, source.begin(), source.end() );
        }
        
        //! push back (ok memory version) a range to target
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
        
        //! push back (ok memory version) a source into target
        template <typename TARGET, typename SOURCE> static inline
        void push_back_(TARGET &target, SOURCE &source )
        {
            push_back(target, source.begin(), source.end() );
        }
        
        //! push front a range to target
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
        
        //! push front a source into target
        template <typename TARGET, typename SOURCE> static inline
        void push_front(TARGET &target, SOURCE &source )
        {
            push_front(target, source.begin(), source.end() );
        }
        
        
        
    };
}

#endif

