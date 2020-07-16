
#include "y/concurrent/executor.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{
    namespace concurrent
    {
        executor:: ~executor() throw() {}
        executor::  executor() throw() {}

        void executor:: free() throw()
        {
            executor &self = *this;
            size_t    i    = num_threads();
            while(i-- > 0)
            {
                self(i).free();
            }
        }

        void executor:: release() throw()
        {
            executor &self = *this;
            size_t    i    = num_threads();
            while(i-- > 0)
            {
                self(i).release();
            }
        }

        parallel::cache_type & executor:: operator()(const size_t context_index) throw()
        {
            assert(context_index<num_threads());
            return get_context(context_index).cache;
        }

        const parallel::cache_type & executor:: operator()(const size_t context_index) const throw()
        {
            assert(context_index<num_threads());
            return aliasing::_( *this ).get_context(context_index).cache;
        }
        

        parallel & executor:: operator[](const size_t context_index) throw()
        {
            assert(context_index<num_threads());
            return get_context(context_index);
        }

        const parallel & executor:: operator[](const size_t context_index) const throw()
        {
            assert(context_index<num_threads());
            executor &self = aliasing::_( *this );
            return    self.get_context(context_index);
        }


    }
}
