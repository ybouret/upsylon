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

        number_type::  number_type() throw() {}
        number_type:: ~number_type() throw() {}
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
            Y_MPN_CHECK(this);
            mgr.__release(byte,allocated);
            bytes=0;
            item =0;
        }

        natural:: natural(const natural &other) : Y_MPN_CTOR(other.bytes,bytes)
        {
            Y_MPN_CHECK(&other); memcpy(byte,other.byte,bytes); Y_MPN_CHECK(this);
        }

        natural:: natural(word_t w) : Y_MPN_CTOR(0,sizeof(word_t))
        {
            memcpy(byte,prepare(w,bytes),sizeof(word_t)); Y_MPN_CHECK(this);
        }

        
        natural:: natural( const void  *buffer, size_t buflen) : Y_MPN_CTOR(buflen,buflen)
        {
            assert(bytes==buflen);
            memcpy(byte,buffer,buflen);
            update();
        }

        natural & natural:: operator=( const natural &other )
        {
            natural tmp(other); xch(tmp); return *this;
        }

        natural & natural:: operator=( const word_t w )
        {
            natural tmp(w); xch(tmp); return *this;
        }

        void natural:: xch( natural &other ) throw()
        {
            cswap(bytes,other.bytes); cswap(allocated,other.allocated);
            cswap(byte,other.byte);   cswap(item,other.item);
        }

        word_t natural:: lsw() const throw()
        {
            word_t       w = 0;
            const size_t n = min_of(bytes,sizeof(word_t));
            for(size_t i=n;i>0;--i)
            {
                (w <<= 8) |= item[i];
            }
            return w;
        }

        const uint8_t * natural:: prepare( word_t &w, size_t &wb ) throw()
        {
            w = swap_le(w);
            const uint8_t *p = (const uint8_t*)&w;
            const uint8_t *q = p-1;
            wb = sizeof(word_t);
            while(wb>0&&q[wb]<=0)
            {
                --wb;
            }
            return p;
        }

        void natural:: clr() throw() { bytes = 0; memset(byte,0,allocated); }

        size_t natural:: bits() const throw()
        {
            return (bytes<=0) ? 0 : ( (bytes-1) << 3 ) + bits_table::count_for_byte[ item[bytes] ];
        }

        const void  * natural:: ro() const throw() { return byte;   }

        size_t natural:: length() const throw() { return bytes;  }

        std::ostream & operator<<( std::ostream &os, const natural &n)
        {
            return n.display(os);
        }

        bool natural:: is_zero() const throw()     { return (bytes<=0); }

        bool natural:: is_positive() const throw() { return (bytes>0);  }

        bool natural:: is_byte(const uint8_t x) const throw() { return (x<=0) ? (bytes<=0) : ((1==bytes) && (x==byte[0])); }

        void natural:: set_byte(const uint8_t x) throw()
        {
            if(x<=0)
            {
                bytes=0;
            }
            else
            {
                bytes   = 1;
                byte[0] = x;
            }
        }

        bool natural:: is_even() const throw()
        {
            return (bytes<=0) || ( 0 == (byte[0] & 0x01) );
        }

        bool natural:: is_odd() const throw()
        {
            return (bytes>0) && ( 0 != (byte[0]&0x01) );
        }

        int natural:: compare_blocks(const uint8_t *l,
                                     const size_t   nl,
                                     const uint8_t *r,
                                     const size_t   nr) throw()
        {
            assert(l);assert(r);
            if(nl<nr)      { return -1; }
            else if(nr<nl) { return  1; }
            else
            {
                assert(nr==nl);
                size_t i=nl;
                while(i-->0)
                {
                    const uint8_t L = l[i];
                    const uint8_t R = r[i];
                    if(L<R) return -1; else if(R<L) return 1; // else continue
                }
                return 0;
            }
        }

    }
}


