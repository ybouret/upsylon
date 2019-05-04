#include "y/mpl/natural.hpp"
#include "y/exception.hpp"
#include "y/memory/vein.hpp"
#include "y/memory/io.hpp"

namespace upsylon
{
    namespace mpl
    {
        namespace
        {
            static uint64_t ___IO[ Y_U64_FOR_ITEM(memory::vein) ];
            static inline
            memory::allocator * __IO() throw()
            {
                return memory::io::__force<memory::vein>(___IO);
            }
        }

        manager:: manager()
        {
            new ( __IO() ) memory::vein();
        }

        manager:: ~manager() throw()
        {
            destruct( __IO() );
        }

        void * manager:: acquire( size_t &n )
        {
            static memory::allocator &IO = *__IO();
            Y_LOCK(access);
            return IO.acquire(n);
        }

        void   manager:: release(void * &p, size_t &n ) throw()
        {
            static memory::allocator &IO = *__IO();
            Y_LOCK(access);
            IO.release(p,n);
        }

        uint8_t * manager:: __acquire(size_t &n)
        {
            static memory::allocator &IO = *__IO();
            Y_LOCK(access);
            return static_cast<uint8_t*>(IO.acquire(n));
        }

        void manager:: __release(uint8_t * &p,size_t &n) throw()
        {
            static memory::allocator &IO = *__IO();
            Y_LOCK(access);
            IO.release((void * &)p,n);
        }

        base_class::  base_class() throw() {}
        base_class:: ~base_class() throw() {}
    }
}

namespace upsylon
{
    namespace mpl
    {
        natural:: natural() : Y_MPN_CTOR(0,0) { Y_MPN_CHECK(this); }
        natural:: natural(size_t n, const as_capacity_t &) : Y_MPN_CTOR(0,n) { Y_MPN_CHECK(this); }

        natural:: ~natural() throw()
        {
            static manager &mgr = manager::location();
            Y_MPN_CHECK(this); mgr.__release(byte,allocated);
        }

        natural:: natural(const natural &other) : Y_MPN_CTOR(other.bytes,bytes)
        {
            Y_MPN_CHECK(&other); memcpy(byte,other.byte,bytes); Y_MPN_CHECK(this);
        }

        natural:: natural(word_t w) : Y_MPN_CTOR(0,sizeof(word_t))
        {
            memcpy(byte,prepare(w,bytes),sizeof(word_t)); Y_MPN_CHECK(this);
        }

        

    }
}


