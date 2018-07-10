//! \file
#ifndef Y_MP_NATURAL_INCLUDED
#define Y_MP_NATURAL_INCLUDED 1

#include "y/memory/dyadic.hpp"
#include "y/memory/buffer.hpp"
#include "y/os/endian.hpp"

namespace upsylon
{
    namespace mpl
    {

#define Y_MPN_CHECK(PTR)                       \
assert( is_a_power_of_two((PTR)->allocated) ); \
assert( (PTR)->bytes<=(PTR)->allocated );      \
assert( (PTR)->byte-1==(PTR)->item );          \
assert( (0 == (PTR)->bytes) || (PTR)->item[ (PTR)->bytes ] >0 )

#define Y_MPN_CTOR(SZ,MX) bytes(SZ), allocated(MX), byte( __acquire(allocated) ), item(byte-1)

        class natural : public memory::ro_buffer
        {
        public:
            typedef uint64_t word_type;

            //! a zero natural
            inline natural() : Y_MPN_CTOR(0,0) { Y_MPN_CHECK(this); }

            //! a zero with any memory
            inline natural(size_t n, const as_capacity_t &) : Y_MPN_CTOR(0,n) { Y_MPN_CHECK(this); }

            //! release resources
            inline virtual ~natural() throw()
            {
                static memory::allocator &hmem = memory::dyadic::location();
                Y_MPN_CHECK(this);
                hmem.release((void * &)byte,allocated);
            }

            //! copy
            inline natural(const natural &other) : Y_MPN_CTOR(other.bytes,bytes)
            {
                Y_MPN_CHECK(&other);
                memcpy(byte,other.byte,bytes);
                Y_MPN_CHECK(this);
            }

            //! copy from a word_type
            inline natural(word_type n) : Y_MPN_CTOR(sizeof(word_type),bytes)
            {
                n = swap_le(n);
                memcpy(byte,&n,sizeof(word_type));
                update();
            }

            //! no throw swap
            inline void swap_with( natural &other ) throw()
            {
                cswap(bytes,other.bytes);
                cswap(allocated,other.allocated);
                cswap(byte,other.byte);
                cswap(item,other.item);
            }

            //! buffer interface : ro
            inline virtual void  *ro() const throw()     { return byte;   }

            //! buffer interface : length
            inline virtual size_t length() const throw() { return bytes;  }

            //! comparison
            static inline const uint8_t * prepare( word_type &w, size_t &wb ) throw()
            {
                w = swap_le(w);
                const uint8_t *p = (const uint8_t*)&w;
                const uint8_t *q = p-1;
                wb = sizeof(word_type);
                while(wb>0&&q[wb]<=0)
                {
                    --wb;
                }
                return p;
            }

            static inline
            int compare_blocks(const uint8_t *l,
                               const size_t   nl,
                               const uint8_t *r,
                               const size_t   nr) throw()
            {
                assert(l);assert(r);
                return (nl<=nr) ? compare_blocks_(l, nl, r, nr) : -compare_blocks_(r, nr, l, nl);
            }

            static inline
            int compare( const natural &lhs, const natural &rhs ) throw()
            {
                return compare_blocks(lhs.byte,lhs.bytes,rhs.byte,rhs.bytes);
            }

            static inline
            int compare( const natural &lhs, word_type w ) throw()
            {
                size_t         nw = 0;
                const uint8_t *pw = prepare(w,nw);
                return compare_blocks(lhs.byte,lhs.bytes,pw,nw);
            }

            static inline
            int compare( word_type w, const natural &rhs ) throw()
            {
                size_t         nw = 0;
                const uint8_t *pw = prepare(w,nw);
                return compare_blocks(pw,nw,rhs.byte,rhs.bytes);
            }

#define Y_MPN_CMP(OP) \
inline friend bool operator OP ( const natural  &lhs, const natural  &rhs ) throw() { return compare(lhs,rhs) OP 0; } \
inline friend bool operator OP ( const natural  &lhs, const word_type rhs ) throw() { return compare(lhs,rhs) OP 0; } \
inline friend bool operator OP ( const word_type lhs, const natural  &rhs ) throw() { return compare(lhs,rhs) OP 0; }

            Y_MPN_CMP(==)
            Y_MPN_CMP(!=)
            Y_MPN_CMP(<=)
            Y_MPN_CMP(<)
            Y_MPN_CMP(>=)
            Y_MPN_CMP(>)

        private:
            size_t   bytes;     //!< active bytes
            size_t   allocated; //!< allocated bytes
            uint8_t *byte;      //!< byte[0..allocated-1]
            uint8_t *item;      //!< item[1..allocated]

            inline void update() throw()
            {
                while(bytes>0&&item[bytes]<=0) --bytes;
            }

            inline void upgrade() throw()
            {
                bytes = allocated;
                update();
            }

            static inline uint8_t * __acquire(size_t &n)
            {
                static memory::allocator &hmem = memory::dyadic::instance();
                return static_cast<uint8_t*>( hmem.acquire(n) );
            }

            static inline int compare_blocks_(const uint8_t *small_data,
                                              const size_t   small_size,
                                              const uint8_t *large_data,
                                              const size_t   large_size) throw()
            {
                assert(small_size<=large_size);
                for(size_t i=0; i<small_size;++i)
                {
                    const uint8_t s = small_data[i];
                    const uint8_t l = large_data[i];
                    if(s<l)
                    {
                        return -1;
                    }
                    else if(l<s)
                    {
                        return 1;
                    }
                    // else continue
                }
                return (small_size<large_size) ? 1 : 0;
            }


        };
    }

    typedef mpl::natural mpn;
}
#endif

