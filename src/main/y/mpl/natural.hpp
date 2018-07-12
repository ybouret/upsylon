//! \file
#ifndef Y_MP_NATURAL_INCLUDED
#define Y_MP_NATURAL_INCLUDED 1

#include "y/memory/dyadic.hpp"
#include "y/memory/buffer.hpp"
#include "y/os/endian.hpp"
#include "y/comparison.hpp"
#include "y/code/utils.hpp"
#include <iostream>

namespace upsylon
{
    namespace mpl
    {

        //! check natural sanity
#define Y_MPN_CHECK(PTR)                       \
assert( is_a_power_of_two((PTR)->allocated) ); \
assert( (PTR)->bytes<=(PTR)->allocated );      \
assert( (PTR)->byte-1==(PTR)->item );          \
assert( (0 == (PTR)->bytes) || (PTR)->item[ (PTR)->bytes ] >0 )

        //! in place constructor
#define Y_MPN_CTOR(SZ,MX) memory::ro_buffer(), bytes(SZ), allocated(MX), byte( __acquire(allocated) ), item(byte-1)

        //! big natural number
        class natural : public memory::ro_buffer
        {
        public:
            typedef uint64_t word_type; //!< integral type for drop in replacement

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

            //! assign
            natural & operator=( const natural &other )
            {
                natural tmp(other);
                xch(tmp);
                return *this;
            }

            //! assign from word_type
            natural & operator=( word_type w )
            {
                natural tmp(w);
                xch(tmp);
                return *this;
            }

            //! no throw swap
            inline void xch( natural &other ) throw()
            {
                cswap(bytes,other.bytes);
                cswap(allocated,other.allocated);
                cswap(byte,other.byte);
                cswap(item,other.item);
            }

            //! Least Significant Word
            inline word_type lsw() const throw()
            {
                word_type    w = 0;
                const size_t n = min_of(bytes,sizeof(word_type));
                for(size_t i=n;i>0;--i)
                {
                    (w <<= 8) |= item[i];
                }
                return w;
            }
            
            //! buffer interface : ro
            inline virtual const void  *ro() const throw()     { return byte;   }

            //! buffer interface : length
            inline virtual size_t length() const throw() { return bytes;  }

            //! prepare a scalar type
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

            //! display hex
            inline std::ostream & to_hex( std::ostream &os ) const
            {
                os << '0' << 'x';
                if(bytes<=0) os << hexadecimal::lowercase[0];
                else
                {
                    for(size_t i=bytes;i>0;--i) os << hexadecimal::lowercase[ item[i] ];
                }
                return os;
            }

            //! fast checking against 0
            inline bool is_zero() const throw() { return (bytes<=0); }

            //! fast checking against a byte
            inline bool is_byte(const uint8_t x) const throw()
            {
                if(x<=0)
                {
                    return (bytes<=0);
                }
                else
                {
                    return (1==bytes) && (x==byte[0]);
                }
            }

            //! fast==1
            inline bool is_one() const throw() { return (1==bytes) && (1==byte[0]);}
            //! fast==2
            inline bool is_two() const throw() { return (1==bytes) && (2==byte[0]);}

            //! get number of bits
            inline size_t bits() const throw()
            {
                return (bytes<=0) ? 0 : ( (bytes-1) << 3 ) + bits_table::count_for_byte[ item[bytes] ];
            }

            //! comparison
            static inline
            int compare_blocks(const uint8_t *l,
                               const size_t   nl,
                               const uint8_t *r,
                               const size_t   nr) throw()
            {
                assert(l);assert(r);
                if(nl<nr)
                {
                    return -1;
                }
                else if(nr<nl)
                {
                    return 1;
                }
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

            //! inline preparation of a word
#define Y_MPN_PREPARE(W) size_t nw = 0; const uint8_t *pw = prepare(W,nw)
            //! multiple overloaded prototypes, no throw
#define Y_MPN_DEFINE_NOTHROW(RET,BODY,CALL) \
static inline RET BODY(const natural &lhs, const natural &rhs) throw() { return CALL(lhs.byte,lhs.bytes,rhs.byte,rhs.bytes);      }\
static inline RET BODY(const natural &lhs, word_type      w  ) throw() { Y_MPN_PREPARE(w); return CALL(lhs.byte,lhs.bytes,pw,nw); }\
static inline RET BODY(word_type      w,   const natural &rhs) throw() { Y_MPN_PREPARE(w); return CALL(pw,nw,rhs.byte,rhs.bytes); }

            Y_MPN_DEFINE_NOTHROW(int,compare,compare_blocks)

            //! comparison operator declarations
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

            //! multiple overloaded prototypes
#define Y_MPN_DEFINE(RET,BODY) \
static inline RET BODY(const natural &lhs, const natural &rhs) { return BODY(lhs.byte,lhs.bytes,rhs.byte,rhs.bytes);      }\
static inline RET BODY(const natural &lhs, word_type      w  ) { Y_MPN_PREPARE(w); return BODY(lhs.byte,lhs.bytes,pw,nw); }\
static inline RET BODY(word_type      w,   const natural &rhs) { Y_MPN_PREPARE(w); return BODY(pw,nw,rhs.byte,rhs.bytes); }

            //! multiple prototype for operators
#define Y_MPN_IMPL(OP,CALL) \
natural & operator OP##=(const natural  &rhs) { natural ans = CALL(*this,rhs); xch(ans); return *this; } \
natural & operator OP##=(const word_type rhs) { natural ans = CALL(*this,rhs); xch(ans); return *this; } \
inline friend natural operator OP ( const natural  &lhs, const natural  &rhs ) { return CALL(lhs,rhs); } \
inline friend natural operator OP ( const natural  &lhs, const word_type rhs ) { return CALL(lhs,rhs); } \
inline friend natural operator OP ( const word_type lhs, const natural  &rhs ) { return CALL(lhs,rhs); }
            //__________________________________________________________________
            //
            // ADD
            //__________________________________________________________________
            Y_MPN_DEFINE(natural,__add)
            Y_MPN_IMPL(+,__add)

            //! unary plus
            natural operator+() { return *this; }

            //! increase by 1
            inline natural __inc() const
            {
                static const uint8_t __one = 0x01;
                return __add(byte,bytes,&__one,1);
            }

            //! increase operator
            inline natural & operator++()
            {
                natural tmp = __inc();
                xch(tmp);
                return *this;
            }

            //! increase operator
            natural operator++(int)
            {
                natural tmp = __inc();
                xch(tmp);
                return tmp;
            }

            //__________________________________________________________________
            //
            // sub
            //__________________________________________________________________
            Y_MPN_DEFINE(natural,__sub)
            Y_MPN_IMPL(-,__sub)

            //! decrease by 1
            inline natural __dec() const
            {
                static const uint8_t __one = 0x01;
                return __sub(byte,bytes,&__one,1);
            }

            //! decrease operator
            inline natural & operator--()
            {
                natural tmp = __dec();
                xch(tmp);
                return *this;
            }

            //! decrease operator
            natural operator--(int)
            {
                natural tmp = __dec();
                xch(tmp);
                return tmp;
            }

            //__________________________________________________________________
            //
            // mul
            //__________________________________________________________________
            Y_MPN_DEFINE(natural,__mul)
            Y_MPN_IMPL(*,__mul)

            //! fast square
            static natural square_of( const natural &n );

            //! ready any byte
            uint8_t operator[](size_t indx) const throw()
            {
                if(indx>=bytes) return 0;
                else return byte[indx];
            }

            //! ready valid bit to 0 or 1
            inline uint8_t get_bit(const size_t ibit) const throw()
            {
                assert(ibit<=bits());
                return bits_table::_true[ byte[ibit>>3] & bits_table::value[ibit&7] ];
            }

            //! test a valid bit
            inline bool has_bit(const size_t ibit) const throw()
            {
                return 0!=(byte[ibit>>3] & bits_table::value[ibit&7]);
            }

            //! left shift
            natural shl(const size_t shift) const
            {
                if(shift>0&&bytes>0)
                {
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
                else
                {
                    return *this;
                }
            }

            //! in place left shift operator
            inline natural & operator<<=(const size_t shift)
            {
                natural ans = shl(shift);
                xch(ans);
                return *this;
            }

            //! left shift operator
            inline friend natural operator<<(const natural &n,const size_t shift)
            {
                const natural ans = n.shl(shift);
                return ans;
            }

            //! right shift
            inline natural shr(const size_t shift) const
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

            //! in place right shift operator
            inline natural & operator>>=(const size_t shift)
            {
                natural ans = shr(shift);
                xch(ans);
                return *this;
            }

            //! left shift operator
            inline friend natural operator>>(const natural &n, const size_t shift)
            {
                const natural ans = n.shr(shift);
                return ans;
            }


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


            static natural __add(const uint8_t *l,
                                 const size_t   nl,
                                 const uint8_t *r,
                                 const size_t   nr);

            static natural __sub(const uint8_t *l,
                                 const size_t   nl,
                                 const uint8_t *r,
                                 const size_t   nr);

            static natural __mul(const uint8_t *l, const size_t nl,
                                 const uint8_t *r, const size_t nr);
        };
    }

    typedef mpl::natural   mpn; //!< alias
}
#endif

