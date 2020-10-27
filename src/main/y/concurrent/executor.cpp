
#include "y/concurrent/executor.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{
    namespace concurrent
    {
        executor:: ~executor() throw() {}
        executor::  executor() throw() {}

        
        void executor:: acquire(const size_t n)
        {
            executor     &self = *this;
            const size_t  nthr = size();
            for(size_t i=1;i<=nthr;++i)
            {
                self[i].acquire(n);
            }
        }

        void executor:: free() throw()
        {
            executor &self = *this;
            for(size_t i=size();i>0;--i)
            {
                self[i].free();
            }
        }

        void executor:: release() throw()
        {
            executor &self = *this;
            for(size_t i=size();i>0;--i)
            {
                self[i].release();
            }
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
