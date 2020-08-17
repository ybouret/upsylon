
//! \file

/**
 Simple Inline Bignum Uspylon Library
 */
#ifndef Y_SIBYL_NATURAL_INCLUDED
#define Y_SIBYL_NATURAL_INCLUDED 1

#include "y/sibyl/number.hpp"
#include "y/code/round.hpp"
#include "y/code/utils.hpp"
#include "y/os/endian.hpp"
#include "y/randomized/bits.hpp"
#include "y/os/real-time-clock.hpp"

namespace upsylon {


    namespace sibyl {

        //! constructor setup to prepare for BYTES
#define Y_MPN_CTOR(BYTES)           \
number(),                           \
bytes(0),                           \
words(0),                           \
count( words_for(BYTES) ),          \
width(0),                           \
shift(0),                           \
word( acquire(count,width,shift) )


#if !defined(NDEBUG)
        //! full consistency checking
#define Y_MPN_CHECK(HOST)\
/**/ do {                                                      \
/**/     assert( (HOST).words ==  words_for( (HOST).bytes ) ); \
/**/     assert( (HOST).words <= (HOST).count );               \
/**/     assert( (HOST).bytes <= (HOST).width );               \
/**/     if( (HOST).bytes > 0 )                                \
/**/         assert( 0 != (HOST).get((HOST).bytes-1) );        \
/**/     for(size_t r=(HOST).bytes;r<(HOST).width;++r)         \
/**/         assert( (HOST).get(r) == 0 );                     \
/**/ } while(false)
#else
#define Y_MPN_CHECK(HOST)
#endif

#define Y_MPN_ADD_TMX


        //______________________________________________________________________
        //
        //
        //! natural little endian
        //
        //______________________________________________________________________
        template <size_t WORD_BITS>
        class natural : public number
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            static const size_t                            word_bits = Y_ROUND8(WORD_BITS);              //!< aligned WORD_BITS
            static const size_t                            word_size = word_bits >> 3;                   //!< sizeof(word)
            static const size_t                            word_exp2 = ilog2<word_size>::value;          //!< word_size = 1 << word_exp2
            static const size_t                            word_mask = word_size-1;                      //!< to compute modulo within a word
            typedef typename unsigned_int<word_size>::type word_type;                                    //!< external word to work with
            static  const size_t                           min_core_size = word_size << 1;               //!< internal core to compute with
            static const word_type                         max_word      = limit_of<word_type>::maximum; //!< single word max value
            //! try to use system core type, otherwise fallback on largest type
            static  const size_t                           core_size     = (min_core_size>sys_core_size) ? max_core_size : sys_core_size;
            //! validate size
            static  const size_t                           find_size     = (core_size>=min_core_size) ? core_size : 0;
            typedef typename unsigned_int<find_size>::type core_type;                                    //!< internal core_type
            typedef uint64_t                               utype;                                        //!< unsigned integral type
            static  const size_t                           words_per_utype = sizeof(utype) >> word_exp2; //!< utype <=> words

            static uint64_t add_ticks;

            //__________________________________________________________________
            //
            // C++ with different constructors/assigns...
            //__________________________________________________________________

            //! cleanup
            inline virtual ~natural() throw()
            {
                release(word,count,width, shift);
                bytes=words=0;
            }

            //! default constructor: 0
            inline  natural() : Y_MPN_CTOR(0) { Y_MPN_CHECK(*this); }

            //! default constructor with some capacity
            inline  natural(const size_t n, const as_capacity_t &) : Y_MPN_CTOR(n) { Y_MPN_CHECK(*this); }

            //! construct from an integral type
            inline natural(utype u) : Y_MPN_CTOR(sizeof(utype))
            {
                Y_MPN_CHECK(*this);
                for(size_t iw=0;iw<words_per_utype;++iw)
                {
                    word[iw] = word_type(u);
                    u >>= word_bits;
                }
                upgrade();
                Y_MPN_CHECK(*this);
            }

            //! copy constructor
            inline natural(const natural &other) :
            number(),
            bytes(other.bytes),
            words(other.words),
            count(words),
            width(0),
            shift(0),
            word(acquire(count,width,shift) )
            {
                Y_MPN_CHECK(other);
                memcpy(word,other.word,bytes);
                Y_MPN_CHECK(*this);
            }

            //! assign by copy/xch
            inline natural & operator=(const natural &other)
            {
                Y_MPN_CHECK(other);
                natural tmp(other); xch(tmp);
                Y_MPN_CHECK(*this);
                return *this;
            }
            
            //! assign by copy/xch
            inline natural & operator=(const utype _)
            {
                natural tmp(_); xch(tmp);
                Y_MPN_CHECK(*this);
                return *this;
            }

            //! build from random bits
            inline natural( randomized::bits &ran, const size_t bits ) : Y_MPN_CTOR( Y_ROUND8(bits)>>3 )
            {
                Y_MPN_CHECK(*this);
                if(bits)
                {
                    size_t ibit = bits-1;  // must be set to 1
                    size_t imsb = ibit>>3; // at this byte
                    // prepare MSB
                    {
                        uint8_t &b = get(imsb);
                        ibit      &= 7;
                        b = bits_table::value[ibit];
                        while(ibit-- > 0)
                        {
                            if(ran.choice()) b |= bits_table::value[ibit];
                        }
                    }

                    // fill
                    for(size_t i=0;i<imsb;++i)
                    {
                        get(i) = ran.full<uint8_t>();
                    }

                    // update status
                    bytes = ++imsb;
                    words = words_for(bytes);
                    Y_MPN_CHECK(*this);
                }
            }



            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! set to zero
            inline void ldz() throw()
            {
                Y_MPN_CHECK(*this);
                memset(word,0,width);
                bytes=0;
                words=0;
                Y_MPN_CHECK(*this);
            }

            //! no-throw exchange
            inline void xch(natural &other) throw()
            {
                Y_MPN_CHECK(other);
                Y_MPN_CHECK(*this);
                cswap(bytes,other.bytes);
                cswap(count,other.count);
                cswap(width,other.width);
                cswap(shift,other.shift);
                cswap(words,other.words);
                cswap(word, other.word );
                Y_MPN_CHECK(other);
                Y_MPN_CHECK(*this);
            }




            //! display value
            inline void  display(std::ostream &os) const
            {
                Y_MPN_CHECK(*this);
                if(bytes>0)
                {
                    for(size_t i=bytes;i>0;)
                    {
                        --i;
                        os<< hexadecimal::lowercase[ get(i) ];
                    }
                }
                else
                {
                    os << '0';
                }
            }

            //! output operator
            inline friend std::ostream & operator<<( std::ostream &os, const natural &n )
            {
                n.display(os);
                return os;
            }

            //! get least significant words
            utype lsw() const throw()
            {
                Y_MPN_CHECK(*this);
                utype ans = 0;
                for(size_t i=words_per_utype;i>0;)
                {
                    ans <<= word_bits;
                    ans |=  utype(word[--i]);
                }
                return ans;
            }

            //! compute number of bits
            inline size_t bits() const throw()
            {
                if(bytes<=0)
                {
                    return 0;
                }
                else
                {
                    const size_t  bm1 = bytes-1;
                    const uint8_t msb = get(bm1);
                    assert(msb);
                    return (bm1 << 3) + bits_table::count_for_byte[ msb ];
                }
            }

            //! get bit flag at position ibit<bits()
            inline bool get_bit(const size_t ibit) const throw()
            {
                assert(ibit<bits());
                const uint8_t b = get(ibit>>3);
                return 0 != (b&bits_table::value[ibit&7]);
            }

            //! locally prepare an utype into a nw words at pw
#define Y_MPN_U2W(ARGS) volatile utype _ = (ARGS); size_t nw = 0; const word_type *pw=u2w((utype&)_,nw)

            //! wrap the different calls
#define Y_MPN_WRAP_API(CALL) \
static inline natural CALL(const natural &lhs, const natural &rhs) { return CALL(lhs.word,lhs.words,rhs.word,rhs.words);            }\
static inline natural CALL(const natural &lhs, const utype    rhs) { Y_MPN_U2W(rhs); return CALL(lhs.word,lhs.words,pw,nw); }\
static inline natural CALL(const utype    lhs, const natural &rhs) { Y_MPN_U2W(lhs); return CALL(pw,nw,rhs.word,rhs.words); }


            //! wrap the different binary calls
#define Y_MPN_WRAP_OPS(OP,CALL) \
inline natural & operator OP##= (const natural &rhs) { natural tmp = CALL(*this,rhs); xch(tmp); return *this; }\
inline natural & operator OP##= (utype          rhs) { natural tmp = CALL(*this,rhs); xch(tmp); return *this; }\
inline friend  natural operator OP (const natural &lhs, const natural &rhs) { return CALL(lhs,rhs); }\
inline friend  natural operator OP (const utype    lhs, const natural &rhs) { return CALL(lhs,rhs); }\
inline friend  natural operator OP (const natural &lhs, const utype    rhs) { return CALL(lhs,rhs); }\

            //__________________________________________________________________
            //
            // addition
            //__________________________________________________________________
            Y_MPN_WRAP_API(add)
            Y_MPN_WRAP_OPS(+,add)

            //! unary '+'
            inline natural operator+() const
            {
                return *this;
            }

            //! prefix++ operator
            inline natural & operator++()
            {
                static word_type one = 1;
                natural          tmp = add(&one,1,word,count);
                xch(tmp); return *this;
            }

            //! postfix++ operator
            inline natural  operator++(int)
            {
                static word_type one = 1;
                const  natural   ans(*this);
                {
                    natural          tmp = add(&one,1,word,count);
                    xch(tmp);
                }
                return ans;
            }

            //! wrap the different calls
#define Y_MPN_WRAP_SAFE_API(RETURN,FUNCTION) \
static inline RETURN FUNCTION(const natural &lhs, const natural &rhs) throw() { return FUNCTION(lhs.word,lhs.words,rhs.word,rhs.words);  }\
static inline RETURN FUNCTION(const natural &lhs, const utype    u  ) throw() { Y_MPN_U2W(u); return FUNCTION(lhs.word,lhs.words,pw,nw); }\
static inline RETURN FUNCTION(const utype    u,   const natural &rhs) throw() { Y_MPN_U2W(u); return FUNCTION(pw,nw,rhs.word,rhs.words); }

            //! wrap the different boolean calls
#define Y_MPN_WRAP_SAFE_CMP(OP,CALL) \
inline friend bool operator OP (const natural &lhs, const natural &rhs) throw() { return CALL(lhs,rhs); } \
inline friend bool operator OP (const natural &lhs, const utype    rhs) throw() { return CALL(lhs,rhs); } \
inline friend bool operator OP (const utype    lhs, const natural &rhs) throw() { return CALL(lhs,rhs); }

            //__________________________________________________________________
            //
            // equality
            //__________________________________________________________________
            Y_MPN_WRAP_SAFE_API(bool,eq)
            Y_MPN_WRAP_SAFE_CMP(==,eq)

            //__________________________________________________________________
            //
            // difference
            //__________________________________________________________________
            Y_MPN_WRAP_SAFE_API(bool,neq)
            Y_MPN_WRAP_SAFE_CMP(!=,neq)


            //__________________________________________________________________
            //
            // helpers
            //__________________________________________________________________

            //! find number of words to hold some bytes
            static inline size_t words_for(const size_t bytes) throw() { return Y_ALIGN_FOR_ITEM(word_type,bytes) >> word_exp2;  }

            //! memory acquire
            static  inline word_type *acquire(size_t &count, size_t &width, size_t &shift)
            {
                static memory_allocator &mgr = instance();
                return mgr.acquire_field<word_type>(count,width,shift);
            }

            //! memory release
            static inline void release(word_type * &w, size_t &count, size_t &width, size_t &shift) throw()
            {
                static memory_allocator &mgr = location();
                mgr.release_field(w,count,width,shift);
            }

            //! utype -> words, and number of words <= words_per_utype
            static inline const word_type *u2w( utype &u, size_t &n ) throw()
            {
                u            = swap_le(u);
                word_type *w = (word_type *)&u;
                n            = words_per_utype;
                size_t m     = words_per_utype-1;
                while(n>0&&w[m]<=0)
                {
                    --n;
                    --m;
                }
                return w;
            }

        private:
            size_t     bytes; //!< active: number of bytes <= width=2^shift
            size_t     words; //!< active: number of words <= count
            size_t     count; //!< memory: max number of words
            size_t     width; //!< memory: width = count * sizeof(word_type), max number of bytes
            size_t     shift; //!< memory: width = 1 << shift
            word_type *word;  //!< address

            //! decrease bytes to first not 0
            inline void update() throw()
            {
                assert(bytes<=width);
                size_t curr = bytes;
                size_t prev = curr-1;
                while(curr>0&&get(prev)<=0)
                {
                    curr = prev--;
                }
                bytes = curr;
                words = words_for(bytes);
            }

            //! set bytes to width and update
            inline void upgrade() throw()
            {
                bytes=width;
                update();
            }

            //__________________________________________________________________
            //
            // addition
            //__________________________________________________________________
            static inline natural add(const word_type *lhs, const size_t nl,
                                      const word_type *rhs, const size_t nr)
            {
                assert(lhs);assert(rhs);
                //--------------------------------------------------------------
                // organize data
                //--------------------------------------------------------------
                const word_type *small_data = lhs;
                size_t           small_size = nl;
                const word_type *large_data = rhs;
                size_t           large_size = nr;
                if(large_size<small_size)
                {
                    cswap(small_data,large_data);
                    cswap(small_size,large_size);
                }
                const size_t num   = large_size+1;
                const size_t space = num << word_exp2;
                natural      ans(space,as_capacity); Y_MPN_CHECK(ans);
#if defined(Y_MPN_ADD_TMX)
                const uint64_t tmx = real_time_clock::ticks();
#endif
                word_type   *sum   = ans.word;
                core_type    carry = 0;

                //--------------------------------------------------------------
                // add for common part
                //--------------------------------------------------------------
                for(size_t i=0;i<small_size;++i)
                {
                    carry += core_type(small_data[i])+core_type(large_data[i]);
                    sum[i] = word_type(carry);
                    carry >>= word_bits;
                    assert(carry<core_type(max_word));
                }

                //--------------------------------------------------------------
                // then propagate carry
                //--------------------------------------------------------------
                for(size_t i=small_size;i<large_size;++i)
                {
                    carry += core_type(large_data[i]);
                    sum[i] = word_type(carry);
                    carry >>= word_bits;
                    assert(carry<core_type(max_word));
                }

                //__________________________________________________________________
                //
                // register carry
                //__________________________________________________________________
                sum[large_size] = word_type(carry);

                ans.bytes = space;
                ans.update();
#if defined(Y_MPN_ADD_TMX)
                add_ticks += real_time_clock::ticks()-tmx;
#endif
                Y_MPN_CHECK(ans);
                return ans;
            }

            //__________________________________________________________________
            //
            // equality
            //__________________________________________________________________
            inline static bool eq(const word_type *lhs, const size_t nl,
                                  const word_type *rhs, const size_t nr) throw()
            {
                assert(rhs); assert(lhs);
                if(nl!=nr)
                    return false;
                else
                {
                    size_t n = nl;
                    while(n-- > 0)
                    {
                        if(lhs[n]!=rhs[n]) return false;
                    }
                    return true;
                }
            }

            inline static bool neq(const word_type *lhs, const size_t nl,
                                   const word_type *rhs, const size_t nr) throw()
            {
                assert(rhs); assert(lhs);
                if(nl!=nr)
                    return true;
                else
                {
                    size_t n = nl;
                    while(n-- > 0)
                    {
                        if(lhs[n]!=rhs[n]) return true;
                    }
                    return false;
                }
            }

            //! access all addressable bytes in 0..width-1
            inline uint8_t &get(const size_t indx) const throw()
            {
                assert(indx<width);
                const word_type &w = word[ indx >> word_exp2];
                uint8_t         *p = (uint8_t *)&w;
                const size_t     i = indx &  word_mask;
#if Y_BYTE_ORDER == Y_BIG_ENDIAN
                return p[word_mask-i];
#else
                return p[i];
#endif
            }
        };

        template <size_t BITS> uint64_t natural<BITS>::add_ticks = 0;

    }

}


#endif

