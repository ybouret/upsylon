
#include "y/concurrent/executor.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{
    namespace concurrent
    {
        executor:: ~executor() throw() {}
        executor::  executor() throw() {}

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

        size_t sequential:: num_threads() const throw() { return 1; }


        void sequential:: run( kernel proc, void *data )
        {
            assert(proc);
            proc(data,context,access);
        }

        parallel & sequential:: get_context(const size_t) throw() { return context; }
        
    }

}
