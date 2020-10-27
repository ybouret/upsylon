
#include "y/concurrent/executor.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{
    namespace concurrent
    {
        executor:: ~executor() throw() {}
        executor::  executor() throw() {}

#if 0
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
#endif
        
#if 0
        void executor:: acquire(const size_t n)
        {
            executor     &self = *this;
            const size_t  nthr = num_threads();
            for(size_t i=0;i<nthr;++i)
            {
                self[i].acquire(n);
            }
        }
#endif

    }
}

namespace upsylon
{
    namespace concurrent
    {
        sequential::sequential() throw() :
        executor(),context(),access()
        {}

        sequential:: ~sequential() throw()
        {
        }

        size_t sequential:: size() const throw() { return 1; }


        void sequential:: run( kernel proc, void *data )
        {
            assert(proc);
            proc(data,context,access);
        }

        parallel & sequential:: operator[](const size_t) throw()
        {
            return context;
        }

        const parallel & sequential:: operator[](const size_t) const throw()
        {
            return context;
        }


    }

}
