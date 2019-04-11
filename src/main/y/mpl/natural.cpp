#include "y/mpl/natural.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace mpl
    {
        manager:: manager() : IO() { }

        manager:: ~manager() throw()  { }

        void * manager:: acquire( size_t &n ) { Y_LOCK(access); return IO.acquire(n); }

        void   manager:: release(void * &p, size_t &n ) throw() { Y_LOCK(access); IO.release(p,n); }

        uint8_t * manager:: __acquire(size_t &n)
        {
            Y_LOCK(access);
            return static_cast<uint8_t*>(IO.acquire(n));
        }

        void manager:: __release(uint8_t * &p,size_t &n) throw()
        {
            Y_LOCK(access);
            IO.release((void * &)p,n);
        }

     
    }
}

