//! \file
#ifndef Y_MP_NATURAL_INCLUDED
#define Y_MP_NATURAL_INCLUDED 1

#include "y/string.hpp"
#include "y/memory/vein.hpp"
#include "y/type/utils.hpp"
#include "y/os/endian.hpp"
#include "y/comparison.hpp"
#include "y/code/utils.hpp"
#include "y/randomized/bits.hpp"
#include "y/ios/ostream.hpp"
#include "y/ios/istream.hpp"

#include <iostream>
#include <cstring>

namespace upsylon
{
    namespace mpl
    {

        typedef uint64_t word_t;    //!< integral type for drop in replacement
        typedef int64_t  integer_t; //!< integral type for
        //! dedicated memory manager
        class manager : public singleton<manager>, public memory::allocator
        {
        public:
            //! allocator interface
            virtual void *acquire( size_t &n ) { Y_LOCK(access); return IO.acquire(n); }

            //! allocator interface
            virtual void release(void * &p, size_t &n ) throw() { Y_LOCK(access); IO.release(p,n); }

            //! specialized acquire
            inline uint8_t * __acquire(size_t &n)
            {
                Y_LOCK(access);
                return static_cast<uint8_t*>(IO.acquire(n));
            }

            //! specialized release
            inline void __release(uint8_t * &p,size_t &n) throw()
            {
                Y_LOCK(access);
                IO.release((void * &)p,n);
            }


        private:
            explicit manager();
            virtual ~manager() throw();
            friend class singleton<manager>;
            memory::vein IO;

        public:
            static const at_exit::longevity life_time = object::life_time - 1; //!< need only objects
        };

        //! check natural sanity
#define Y_MPN_CHECK(PTR)                                        \
assert( is_a_power_of_two((PTR)->allocated) );                  \
assert( (PTR)->bytes<=(PTR)->allocated );                       \
assert( (PTR)->byte-1==(PTR)->item );                           \
assert( (0 == (PTR)->bytes) || (PTR)->item[ (PTR)->bytes ] >0 )

        //! in place constructor
#define Y_MPN_CTOR(SZ,MX) memory::ro_buffer(), ios::serializable(), bytes(SZ), allocated(MX), byte( __acquire(allocated) ), item(byte-1)

        class integer; //!< forward declaration

        //! big natural number
        class natural : public memory::ro_buffer, public ios::serializable
        {
        public:
            static const char CLASS_NAME[]; //!< for serializable
            //__________________________________________________________________
            //
            //
            // setup
            //
            //__________________________________________________________________

            //! a zero natural
            inline natural() : Y_MPN_CTOR(0,0) { Y_MPN_CHECK(this); }

            //! a zero with any memory
            inline natural(size_t n, const as_capacity_t &) : Y_MPN_CTOR(0,n) { Y_MPN_CHECK(this); }

            //! release memory
            inline virtual ~natural() throw()
            {
                static manager &mgr = manager::location();
                Y_MPN_CHECK(this); mgr.__release(byte,allocated);
            }

            //! copy
            inline natural(const natural &other) : Y_MPN_CTOR(other.bytes,bytes)
            {
                Y_MPN_CHECK(&other); memcpy(byte,other.byte,bytes); Y_MPN_CHECK(this);
            }

            //! copy from a word_type
            inline natural(word_t w) : Y_MPN_CTOR(0,sizeof(word_t))
            {
                memcpy(byte,prepare(w,bytes),sizeof(word_t)); Y_MPN_CHECK(this);
            }

            //! copy from raw memory
            inline natural( const void  *buffer, size_t buflen) : Y_MPN_CTOR(buflen,buflen)
            {
                assert(bytes==buflen);
                memcpy(byte,buffer,buflen);
                update();
            }

            //! assign
            natural & operator=( const natural &other )
            {
                natural tmp(other); xch(tmp); return *this;
            }

            //! assign from word_type
            natural & operator=( const word_t w )
            {
                natural tmp(w); xch(tmp); return *this;
            }

            //! no throw exchange
            inline void xch( natural &other ) throw()
            {
                cswap(bytes,other.bytes); cswap(allocated,other.allocated);
                cswap(byte,other.byte);   cswap(item,other.item);
            }

            //! Least Significant Word
            inline word_t lsw() const throw()
            {
                word_t       w = 0;
                const size_t n = min_of(bytes,sizeof(word_t));
                for(size_t i=n;i>0;--i)
                {
                    (w <<= 8) |= item[i];
                }
                return w;
            }
            
            //! buffer interface : ro
            inline virtual const void  *ro() const throw() { return byte;   }

            //! buffer interface : length
            inline virtual size_t length() const throw() { return bytes;  }

            //! prepare a scalar type
            static inline const uint8_t * prepare( word_t &w, size_t &wb ) throw()
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

            //! get number of bits
            inline size_t bits() const throw() { return (bytes<=0) ? 0 : ( (bytes-1) << 3 ) + bits_table::count_for_byte[ item[bytes] ]; }

            //! clear memory
            inline void clr() throw() { bytes = 0; memset(byte,0,allocated); }

            //! generate a random number
            natural(const size_t nbit, randomized::bits &gen );

            //__________________________________________________________________
            //
            //
            // I/O
            //
            //__________________________________________________________________
            //! formatted display
            std::ostream & display( std::ostream &) const;

            //! operator to std::ostream
            inline friend std::ostream & operator<<( std::ostream &os, const natural &n)
            {
                return n.display(os);;
            }

            //! convert to real value
            double to_real() const;

            //! expand the ratio
            static double ratio_of(const natural &num,const natural &den);

            static natural dec( const string &s ); //!< parse decimal string
            static natural hex( const string &s ); //!< parse hexadecimal string

            //__________________________________________________________________
            //
            //
            // comparisons
            //
            //__________________________________________________________________

            //! fast checking against 0
            inline bool is_zero() const throw()     { return (bytes<=0); }

            //! fast checking greater than zero
            inline bool is_positive() const throw() { return (bytes>0);  }

            //! fast checking against a byte
            inline bool is_byte(const uint8_t x) const throw() { return (x<=0) ? (bytes<=0) : ((1==bytes) && (x==byte[0])); }

            //! fast setting to a byte
            inline void set_byte(const uint8_t x) throw()
            {
                if(x<=0)
                {
                    bytes=0;
                }
                else
                {
                    bytes=1;
                    byte[0] = x;
                }
            }

            //! test if even
            inline bool is_even() const throw()
            {
                return (bytes<=0) || ( 0 == (byte[0] & 0x01) );
            }

            //! test if odd
            inline bool is_odd() const throw()
            {
                return (bytes>0) && ( 0 != (byte[0]&0x01) );
            }

            //! comparison
            static inline
            int compare_blocks(const uint8_t *l,
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

            //! inline preparation of a word
#define Y_MPN_PREPARE(W) size_t nw = 0; const uint8_t *pw = prepare(W,nw)
            //! multiple overloaded prototypes, no throw
#define Y_MPN_DEFINE_NOTHROW(RET,BODY,CALL) \
static inline RET BODY(const natural &lhs, const natural &rhs) throw() { return CALL(lhs.byte,lhs.bytes,rhs.byte,rhs.bytes);      }\
static inline RET BODY(const natural &lhs, word_t         w  ) throw() { Y_MPN_PREPARE(w); return CALL(lhs.byte,lhs.bytes,pw,nw); }\
static inline RET BODY(word_t         w,   const natural &rhs) throw() { Y_MPN_PREPARE(w); return CALL(pw,nw,rhs.byte,rhs.bytes); }

            Y_MPN_DEFINE_NOTHROW(int,compare,compare_blocks)

            //! comparison operator declarations
#define Y_MPN_CMP(OP) \
inline friend bool operator OP ( const natural  &lhs, const natural  &rhs ) throw() { return compare(lhs,rhs) OP 0; } \
inline friend bool operator OP ( const natural  &lhs, const word_t    rhs ) throw() { return compare(lhs,rhs) OP 0; } \
inline friend bool operator OP ( const word_t    lhs, const natural  &rhs ) throw() { return compare(lhs,rhs) OP 0; }

            Y_MPN_CMP(==)
            Y_MPN_CMP(!=)
            Y_MPN_CMP(<=)
            Y_MPN_CMP(<)
            Y_MPN_CMP(>=)
            Y_MPN_CMP(>)

            //! multiple overloaded prototypes
#define Y_MPN_DEFINE(RET,BODY) \
static inline RET BODY(const natural &lhs, const natural &rhs) { return BODY(lhs.byte,lhs.bytes,rhs.byte,rhs.bytes);      }\
static inline RET BODY(const natural &lhs, word_t         w  ) { Y_MPN_PREPARE(w); return BODY(lhs.byte,lhs.bytes,pw,nw); }\
static inline RET BODY(word_t         w,   const natural &rhs) { Y_MPN_PREPARE(w); return BODY(pw,nw,rhs.byte,rhs.bytes); }

            //! multiple prototype for operators
#define Y_MPN_IMPL(OP,CALL) \
natural & operator OP##=(const natural  &rhs) { natural ans = CALL(*this,rhs); xch(ans); return *this; } \
natural & operator OP##=(const word_t    rhs) { natural ans = CALL(*this,rhs); xch(ans); return *this; } \
inline friend natural operator OP ( const natural  &lhs, const natural  &rhs ) { return CALL(lhs,rhs); } \
inline friend natural operator OP ( const natural  &lhs, const word_t    rhs ) { return CALL(lhs,rhs); } \
inline friend natural operator OP ( const word_t    lhs, const natural  &rhs ) { return CALL(lhs,rhs); }

            //! declaration and implementation of function for a given operator
#define Y_MPN_WRAP(OP,CALL) Y_MPN_DEFINE(natural,CALL) Y_MPN_IMPL(OP,CALL)

            //__________________________________________________________________
            //
            //
            // ADD
            //
            //__________________________________________________________________
            Y_MPN_WRAP(+,__add)

            //! unary plus
            natural operator+() { return *this; }

            //! increase by 1
            inline natural __inc() const { static const uint8_t __one = 0x01; return __add(byte,bytes,&__one,1); }

            //! increase operator
            inline natural & operator++() { natural tmp = __inc(); xch(tmp); return *this; }

            //! increase operator
            natural operator++(int) { natural tmp = __inc(); xch(tmp); return tmp; }
            
            //__________________________________________________________________
            //
            //
            // SUB
            //
            //__________________________________________________________________
            Y_MPN_WRAP(-,__sub)

            //! decrease by 1
            inline natural __dec() const { static const uint8_t __one = 0x01; return __sub(byte,bytes,&__one,1); }

            //! decrease operator
            inline natural & operator--() { natural tmp = __dec(); xch(tmp); return *this; }

            //! decrease operator
            natural operator--(int) { natural tmp = __dec(); xch(tmp); return tmp; }

            //__________________________________________________________________
            //
            //
            // MUL
            //
            //__________________________________________________________________
            Y_MPN_WRAP(*,__mul)

            //! fast square
            static natural square_of( const natural &n );

            //__________________________________________________________________
            //
            //
            // Bits shifting
            //
            //__________________________________________________________________

            //! ready any byte
            uint8_t operator[](size_t indx) const throw() { return (indx>=bytes) ? 0 : byte[indx]; }

            //! ready valid bit to 0 or 1
            inline uint8_t get_bit(const size_t ibit) const throw() { assert(ibit<=bits()); return bits_table::_true[ byte[ibit>>3] & bits_table::value[ibit&7] ]; }

            //! test a valid bit
            inline bool has_bit(const size_t ibit) const throw() { assert(ibit<=bits()); return 0!=(byte[ibit>>3] & bits_table::value[ibit&7]); }

            //! left shift
            natural shl(const size_t shift) const { return (shift>0&&bytes>0) ? __shl(shift) : *this; }

            //! in place left shift
            natural & shl()
            {
                if(bytes>0) { natural tmp = __shl(1); xch(tmp); }
                return *this;
            }

            //! in place left shift operator
            inline natural & operator<<=(const size_t shift) { natural ans = shl(shift); xch(ans); return *this; }

            //! left shift operator
            inline friend natural operator<<(const natural &n,const size_t shift) { return n.shl(shift); }

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

            //! in place right shift
            natural & shr() { natural tmp = shr(1); xch(tmp); return *this; }

            //! in place right shift operator
            inline natural & operator>>=(const size_t shift) { natural ans = shr(shift); xch(ans); return *this; }

            //! left shift operator
            inline friend natural operator>>(const natural &n, const size_t shift){ return n.shr(shift); }

            //! 2^j
            static inline natural exp2( const size_t j )
            {
                const size_t num_bits  = j+1;
                const size_t new_bytes = Y_BYTES_FOR(num_bits);
                natural ans(new_bytes,as_capacity);
                ans.byte[ j>>3 ] = bits_table::value[j&7];
                ans.bytes = new_bytes;
                assert(ans.item[new_bytes]>0);
                return ans;
            }

            //__________________________________________________________________
            //
            //
            // DIV
            //
            //__________________________________________________________________
            Y_MPN_WRAP(/,__div)
            //! num = q*den+r
            static inline void split( natural &q, natural &r, const natural &num, const natural &den ) { r = num - ( (q=num/den) * den ); }

            //__________________________________________________________________
            //
            //
            // MOD
            //
            //__________________________________________________________________
            Y_MPN_WRAP(%,__mod)

            //__________________________________________________________________
            //
            //
            // DIVISIBILITY
            //
            //__________________________________________________________________

            //! test divisibility
            inline bool is_divisible_by( const natural &rhs ) const { return __dvs(byte,bytes,rhs.byte,rhs.bytes); }

            //! test divisibility
            inline bool is_divisible_by(word_t w) const { Y_MPN_PREPARE(w); return __dvs(byte,bytes,pw,nw); }

            //! test divisibility
            inline bool is_divisible_by_byte(const uint8_t b) const { return __dvs(byte,bytes,&b,1); }

            //__________________________________________________________________
            //
            //
            // boolean ops
            //
            //__________________________________________________________________
            //! byte-wise operator
            typedef uint8_t (*booleanOp)(const uint8_t lhs, const uint8_t rhs);
            Y_MPN_WRAP(&,__and)
            Y_MPN_WRAP(|,__or)
            Y_MPN_WRAP(^,__xor)

            //__________________________________________________________________
            //
            //
            // Arithmetic
            //
            //__________________________________________________________________
            static natural mod_inv( const natural &b, const natural &n );                     //!< modular inverse
            static natural mod_exp( const natural &b, const natural &e, const natural &n );   //!< modular exponentiation (b^e)[n]
            static bool    are_coprimes(const natural &, const natural &); //!< test co-primailty
            static natural factorial(const natural &n); //!< factorial computation, recurvsive algorithm

            //__________________________________________________________________
            //
            //
            // io
            //
            //__________________________________________________________________
            virtual const char *className() const throw(); //!< "mpn"
            virtual size_t      serialize( ios::ostream &fp ) const; //!< write binary
            static natural read( ios::istream &fp, size_t *nr=0); //!< ready binary, number of bytes is saved if possibler

        private:
            size_t   bytes;     //!< active bytes
            size_t   allocated; //!< allocated bytes
            uint8_t *byte;      //!< byte[0..allocated-1]
            uint8_t *item;      //!< item[1..allocated]

            inline void update() throw() { while(bytes>0&&item[bytes]<=0) --bytes; }

            inline void upgrade() throw() { bytes = allocated; update(); }

            static inline uint8_t * __acquire(size_t &n)
            {
                static manager &mgr = manager::instance();
                return mgr.__acquire(n);
            }

            inline natural __shl(const size_t shift) const
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

            static natural __div(const uint8_t *num, const size_t nn,
                                 const uint8_t *den, const size_t nd);

            static natural __mod(const uint8_t *num, const size_t nn,
                                 const uint8_t *den, const size_t nd);

            //! true if divisible
            static bool    __dvs(const uint8_t *num, const size_t nn,
                                 const uint8_t *den, const size_t nd);

            static natural __bool(const uint8_t *l, const size_t nl,
                                  const uint8_t *r, const size_t nr,
                                  booleanOp  proc);
            //! low level boolean functions implementation
#define Y_MPN_BOOL(CALL,OP) \
static inline uint8_t __##CALL##__(const uint8_t a, const uint8_t b ) throw() { return (a OP b); }\
static inline natural __##CALL(const uint8_t *l, const size_t nl, const uint8_t *r, const size_t nr) { return __bool(l,nl,r,nr,__##CALL##__); }
            Y_MPN_BOOL(and,&)
            Y_MPN_BOOL(or,|)
            Y_MPN_BOOL(xor,^)



            friend class integer;
        };

    }
}

#include "y/concurrent/singleton.hpp"
#include "y/sequence/list.hpp"



#include "y/type/xnumeric.hpp"

namespace upsylon
{

    typedef mpl::natural   mpn; //!< alias
    namespace math
    {
        inline mpn fabs_of(const mpn &u) { return u;   } //!< overloaded __fabs function
        inline mpn __mod2(const mpn &u)  { return u*u; } //!< overloaded __mod2 function
    }

    //! extended numeric for mpn
    template <> struct xnumeric<mpn>
    {
        static inline mpn  abs_minimum() { return mpn(); } //!< 0
        static inline bool is_zero(const mpn &x)     { return x.is_zero();     } //!< x==0
        static inline bool is_positive(const mpn &x) { return x.is_positive(); } //!< x>0
    };

}

#include "y/hashing/sha1.hpp"

namespace upsylon
{


    //! precompiled naturals
    class MPN : public singleton<MPN>
    {
    public:
        //! used for list creation
        enum CreateMode
        {
            CreateSafe, //!< always use isPrime_
            CreateFast  //!< use isPrime from freshly computed list
        };
        
        typedef hashing::sha1 Hasher; //!< internal hasher
        
        //! store prime and its squared value
        class PrimeInfo
        {
        public:
            PrimeInfo(const mpn &n);      //!< setup
            PrimeInfo(const PrimeInfo &); //!< hard copy
            ~PrimeInfo() throw();         //!< destructor
            const mpn p;                  //!< a prime
            const mpn q;                  //!< p^2

            //! output just the value
            friend std::ostream & operator<<( std::ostream &os, const PrimeInfo &);

        private:
            Y_DISABLE_ASSIGN(PrimeInfo);
        };

        typedef list<const PrimeInfo> PrimeList; //!< alias
        const PrimeList plist;  //!< precomputed primes, with at least 2 and 3
        const mpn       probe;  //!< next prime probe 5+k*6>=highest prime
        const mpn      _0;      //!< 0
        const mpn      _1;      //!< 1
        const mpn      _2;      //!< 2
        const mpn      _3;      //!< 3
        const mpn      _4;      //!< 4
        const mpn      _5;      //!< 5
        const mpn      _6;      //!< 6
        const mpn      _10;     //!< 10
        
        void   createPrimes( const size_t count,  const CreateMode how=CreateSafe ); //!< append count primes to the primes sequence
        size_t recordPrimes( ios::ostream &fp ) const; //!< serialize
        size_t recordLength() const;                   //!< return the length of the full record
        void   reloadPrimes( ios::istream &fp);        //!< try to reload

        bool isPrime_( const mpn &n ) const;   //!< raw method
        bool isPrime(  const mpn &n ) const;   //!< hybrid method
        mpn  nextPrime_( const mpn &n ) const; //!< raw method
        mpn  nextPrime(  const mpn &n ) const; //!< hybrid method
        
        void   reset() throw(); //!< reset list and probe (failsafe)
        digest md() const;      //!< current digest
        
    private:
        explicit MPN();
        virtual ~MPN() throw();
        friend class singleton<MPN>;
        void   checkList() const;
        size_t nextProbe(const CreateMode how);

    public:
        static const at_exit::longevity life_time = mpl::manager::life_time - 1; //!< based on manager existence
    };

}

#endif

