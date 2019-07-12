//! \file
#ifndef Y_MP_NATURAL_INCLUDED
#define Y_MP_NATURAL_INCLUDED 1

#include "y/strfwd.hpp"
#include "y/type/utils.hpp"
#include "y/os/endian.hpp"
#include "y/randomized/bits.hpp"
#include "y/ios/serializable.hpp"
#include "y/ios/istream.hpp"
#include "y/memory/buffer.hpp"

#include <iostream>

namespace upsylon
{
    namespace mpl
    {

        typedef uint64_t word_t;    //!< integral type for drop in replacement
        typedef int64_t  integer_t; //!< signed integral type

        ////////////////////////////////////////////////////////////////////////
        //
        //! base class to clarify hierarchy
        //
        ////////////////////////////////////////////////////////////////////////
        class number_type : public counted_object
        {
        public:
            explicit number_type() throw(); //!< setup
            virtual ~number_type() throw(); //!< destructor

        private:
            Y_DISABLE_COPY_AND_ASSIGN(number_type);
        };

        ////////////////////////////////////////////////////////////////////////
        //
        //! dedicated memory manager
        //
        ////////////////////////////////////////////////////////////////////////
        class manager : public singleton<manager>, public memory::allocator
        {
        public:
            virtual void *acquire( size_t &n );                    //!< allocator interface
            virtual void  release(void * &p, size_t &n ) throw();  //!< allocator interface
            uint8_t *   __acquire(size_t &n);                      //!< specialized acquire
            void        __release(uint8_t * &p,size_t &n) throw(); //!< specialized release

        private:
            explicit manager();
            virtual ~manager() throw();
            friend class singleton<manager>;

        public:
            static const at_exit::longevity life_time = object::life_time - 1; //!< need only objects
        };

        ////////////////////////////////////////////////////////////////////////
        //
        // multiple precision natural
        //
        ////////////////////////////////////////////////////////////////////////

        //! check natural sanity
#define Y_MPN_CHECK(PTR)                                        \
assert( is_a_power_of_two((PTR)->allocated) );                  \
assert( (PTR)->bytes<=(PTR)->allocated );                       \
assert( (PTR)->byte-1==(PTR)->item );                           \
assert( (0 == (PTR)->bytes) || (PTR)->item[ (PTR)->bytes ] >0 )

        //! in place constructor
#define Y_MPN_CTOR(SZ,MX) object(), number_type(), memory::ro_buffer(), ios::serializable(), bytes(SZ), allocated(MX), byte( __acquire(allocated) ), item(byte-1)

        class integer; //!< forward declaration

        //! big natural number
        class natural : public number_type, public memory::ro_buffer, public ios::serializable
        {
        public:
            static const char CLASS_NAME[]; //!< for serializable
            //__________________________________________________________________
            //
            //
            // virtual interface
            //
            //__________________________________________________________________
            //! buffer interface : ro
            virtual const void  *ro() const throw();

            //! buffer interface : length
            virtual size_t length() const throw();

            //__________________________________________________________________
            //
            //
            // setup
            //
            //__________________________________________________________________

            //! a zero natural
            natural();

            //! a zero with any memory
            natural(size_t n, const as_capacity_t &);

            //! release memory
            virtual ~natural() throw();

            //! copy
            natural(const natural &other);

            //! setup from a word_type
            natural(word_t w);

            //! copy from raw memory
            natural( const void  *buffer, size_t buflen);

            //! assign
            natural & operator=( const natural &other );

            //! assign from word_type
            natural & operator=( const word_t w );

            //! no throw exchange
            void xch( natural &other ) throw();

            //! Least Significant Word
            word_t lsw() const throw();
            
            //! prepare a scalar type
            static  const uint8_t * prepare( word_t &w, size_t &wb ) throw();

            //! get number of bits
            size_t bits() const throw();

            //! clear memory
            void clr() throw();

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
            friend std::ostream & operator<<( std::ostream &os, const natural &n);

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
            bool is_zero() const throw();

            //! fast checking greater than zero
            bool is_positive() const throw();

            //! fast checking against a byte
            bool is_byte(const uint8_t x) const throw();

            //! fast setting to a byte
            void set_byte(const uint8_t x) throw();

            //! test if even
            bool is_even() const throw();

            //! test if odd
            bool is_odd() const throw();

            //! comparison
            static
            int compare_blocks(const uint8_t *l,
                               const size_t   nl,
                               const uint8_t *r,
                               const size_t   nr) throw();

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
            static bool    are_coprimes(const natural &, const natural &);                    //!< test co-primailty
            static natural factorial(const natural &n);                                       //!< factorial computation, recursive algorithm

            //__________________________________________________________________
            //
            //
            // io
            //
            //__________________________________________________________________
            virtual const char *className() const throw();             //!< "mpn"
            virtual size_t      serialize( ios::ostream &fp ) const;   //!< write binary
            static natural      read( ios::istream &fp, size_t *nr=0); //!< read binary, number of bytes is saved if possible

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


#endif

