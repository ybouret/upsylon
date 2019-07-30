#include "y/mpl/natural.hpp"
#include "y/exception.hpp"
#include "y/memory/vein.hpp"
#include "y/memory/io.hpp"
#include "y/type/utils.hpp"

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
            memset(___IO,0,sizeof(___IO));
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

        void natural:: update()  throw() { while(bytes>0&&item[bytes]<=0) --bytes; Y_MPN_CHECK(this); }
        void natural:: upgrade() throw() { bytes = allocated; update();                               }

        uint8_t * natural:: __acquire(size_t &n)
        {
            static manager &mgr = manager::instance();
            return mgr.__acquire(n);
        }

        natural:: natural() : Y_MPN_CTOR(0,0) { Y_MPN_CHECK(this); }
        natural:: natural(size_t n, const as_capacity_t &) : Y_MPN_CTOR(0,n) { Y_MPN_CHECK(this); }

        natural:: ~natural() throw()
        {
            static manager &mgr = manager::location();
            clr();
            mgr.__release(byte,allocated);
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

        natural:: natural(const char   *s) :  Y_MPN_CTOR(0,0)
        {
            Y_MPN_CHECK(this);
            natural _ = parse(s);
            xch(_);
        }

        natural:: natural(const string &s ) : Y_MPN_CTOR(0,0)
        {
            Y_MPN_CHECK(this);
            natural _ = parse(s);
            xch(_);
        }

        natural & natural:: operator=( const char   *s )
        {
            natural _ = parse(s);
            xch(_);
            return *this;
        }

        natural & natural:: operator=( const string &s )
        {
            natural _ = parse(s);
            xch(_);
            return *this;
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
            cswap(bytes,other.bytes);
            cswap(allocated,other.allocated);
            cswap(byte,other.byte);
            cswap(item,other.item);
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

        void natural:: clr() throw()
        {
            Y_MPN_CHECK(this);
            memset(byte,0,bytes);
            bytes = 0;
            Y_MPN_CHECK(this);
        }

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
            Y_MPN_CHECK(this);
            if(x<=0)
            {
                bytes=0;
                memset(byte,0,allocated);
            }
            else
            {
                bytes   = 1;
                byte[0] = x;
                memset(byte+1,0,allocated-1);
            }
            Y_MPN_CHECK(this);
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

        // ADD
        natural natural:: operator+() const { return *this; }

        natural natural:: __inc() const { static const uint8_t __one = 0x01; return __add(byte,bytes,&__one,1); }

        natural & natural:: operator++() { natural tmp = __inc(); xch(tmp); return *this; }

        natural natural:: operator++(int) { natural tmp = __inc(); xch(tmp); return tmp; }

        // SUB
        natural natural:: __dec() const { static const uint8_t __one = 0x01; return __sub(byte,bytes,&__one,1); }

        natural & natural:: operator--() { natural tmp = __dec(); xch(tmp); return *this; }

        natural natural:: operator--(int) { natural tmp = __dec(); xch(tmp); return tmp; }

        // BITS
        uint8_t natural:: operator[](size_t indx) const throw() { return (indx>=bytes) ? 0 : byte[indx]; }

        uint8_t natural:: get_bit(const size_t ibit) const throw() { assert(ibit<=bits()); return bits_table::_true[ byte[ibit>>3] & bits_table::value[ibit&7] ]; }

        bool natural:: has_bit(const size_t ibit) const throw() { assert(ibit<=bits()); return 0!=(byte[ibit>>3] & bits_table::value[ibit&7]); }

        natural natural:: shl(const size_t shift) const { return (shift>0&&bytes>0) ? __shl(shift) : *this; }

        natural & natural:: shl()
        {
            if(bytes>0) { natural tmp = __shl(1); xch(tmp); }
            return *this;
        }

        natural & natural:: operator<<=(const size_t shift) { natural ans = shl(shift); xch(ans); return *this; }

        natural operator<<(const natural &n,const size_t shift) { return n.shl(shift); }

        natural natural:: shr(const size_t shift) const
        {
            const size_t old_bits = bits();
            if(shift>=old_bits)
            {
                return natural();
            }
            else
            {
                const size_t new_bits  = old_bits - shift;
                const size_t new_bytes = Y_BYTES_FOR(new_bits);
                natural ans(new_bytes,as_capacity);
                ans.bytes = new_bytes;

                for(size_t i=shift,j=0;i<old_bits;++i,++j)
                {
                    if(has_bit(i))
                    {
                        ans.byte[j>>3] |= bits_table::value[j&7];
                    }
                }
                assert(ans.item[new_bytes]>0);
                return ans;
            }
        }

        natural & natural:: shr() { natural tmp = shr(1); xch(tmp); return *this; }

        natural & natural:: operator>>=(const size_t shift) { natural ans = shr(shift); xch(ans); return *this; }

        natural operator>>(const natural &n, const size_t shift){ return n.shr(shift); }

        natural natural:: exp2( const size_t j )
        {
            const size_t num_bits  = j+1;
            const size_t new_bytes = Y_BYTES_FOR(num_bits);
            natural ans(new_bytes,as_capacity);
            ans.byte[ j>>3 ] = bits_table::value[j&7];
            ans.bytes = new_bytes;
            assert(ans.item[new_bytes]>0);
            return ans;
        }

        natural natural:: __shl(const size_t shift) const
        {
            assert(shift>0);
            assert(bytes>0);
            const size_t old_bits  = bits();
            const size_t new_bits  = old_bits + shift;
            const size_t new_bytes = Y_BYTES_FOR(new_bits);
            natural ans(new_bytes,as_capacity);
            ans.bytes = new_bytes;

            for(size_t i=0,j=shift;i<old_bits;++i,++j)
            {
                if(has_bit(i))
                {
                    ans.byte[j>>3] |= bits_table::value[j&7];
                }
            }
            assert(ans.item[new_bytes]>0);
            return ans;
        }

        // DVS
        bool natural:: is_divisible_by( const natural &rhs ) const { return __dvs(byte,bytes,rhs.byte,rhs.bytes); }

        bool natural:: is_divisible_by(word_t w) const { Y_MPN_PREPARE(w); return __dvs(byte,bytes,pw,nw); }

        bool natural:: is_divisible_by_byte(const uint8_t b) const { return __dvs(byte,bytes,&b,1); }

        void natural::  split( natural &q, natural &r, const natural &num, const natural &den ) { r = num - ( (q=num/den) * den ); }

    }
}


