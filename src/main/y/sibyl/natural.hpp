
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

namespace upsylon {


    namespace sibyl {

        
#define Y_SIBYL_NATURAL_CTOR(BYTES) \
number(),                           \
bytes(0),                           \
words(0),                           \
count( words_for(BYTES) ),          \
width(0),                           \
shift(0),                           \
word( acquire(count,width,shift) )

#if !defined(NDEBUG)
#define Y_SIBYL_NATURAL_CHECK(HOST) do { \
assert( (HOST).words ==  words_for( (HOST).bytes ) );\
assert( (HOST).words <= (HOST).count ); \
assert( (HOST).bytes <= (HOST).width ); \
if( (HOST).bytes > 0 ) assert( 0 != (HOST).get((HOST).bytes-1) );\
for(size_t remaining=(HOST).bytes;remaining<(HOST).width;++remaining) assert( (HOST).get(remaining) == 0 );\
} while(false)
#else
#define Y_SIBYL_NATURAL_CHECK(HOST)
#endif

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
            static const size_t                            word_bits = Y_ROUND8(WORD_BITS);     //!< aligned WORD_BITS
            static const size_t                            word_size = word_bits >> 3;          //!< sizeof(word)
            static const size_t                            word_exp2 = ilog2<word_size>::value; //!< word_size = 1 << word_exp2
            static const size_t                            word_mask = word_size-1;             //!< to compute modulo within a word
            typedef typename unsigned_int<word_size>::type word_type;                           //!< external word to work with
            static  const size_t                           min_core_size = word_size << 1;      //!< internal core to compute with
            static const word_type                         max_word      = limit_of<word_type>::maximum;
            //! try to use system core type, otherwise fallback on largest type
            static  const size_t                           core_size     = (min_core_size>sys_core_size) ? max_core_size : sys_core_size;
            //! validate size
            static  const size_t                           find_size     = (core_size>=min_core_size) ? core_size : 0;
            typedef typename unsigned_int<find_size>::type core_type; //!< internal core_type
            typedef uint64_t                               utype;     //!< unsigned integral type
            static  const size_t                           words_per_utype = sizeof(utype) >> word_exp2;

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! cleanup
            inline virtual ~natural() throw()
            {
                release(word,count,width, shift);
                bytes=words=0;
            }

            //! default constructor: 0
            inline  natural() : Y_SIBYL_NATURAL_CTOR(0) { Y_SIBYL_NATURAL_CHECK(*this); }

            //! default constructor with some capacity
            inline  natural(const size_t n, const as_capacity_t &) : Y_SIBYL_NATURAL_CTOR(n) { Y_SIBYL_NATURAL_CHECK(*this); }

            //! construct from an integral type
            inline natural(utype u) : Y_SIBYL_NATURAL_CTOR(sizeof(utype))
            {
                Y_SIBYL_NATURAL_CHECK(*this);
                for(size_t iw=0;iw<words_per_utype;++iw)
                {
                    word[iw] = word_type(u);
                    u >>= word_bits;
                }
                upgrade();
                Y_SIBYL_NATURAL_CHECK(*this);
            }

            //! copy constructor
            inline natural(const natural &other) :
            bytes(other.bytes),
            words(other.words),
            count(words),
            width(0),
            shift(0),
            word(acquire(count,width,shift) )
            {
                Y_SIBYL_NATURAL_CHECK(other);
                memcpy(word,other.word,bytes);
                Y_SIBYL_NATURAL_CHECK(*this);
            }

            //! assign by copy/xch
            inline natural & operator=(const natural &other)
            {
                Y_SIBYL_NATURAL_CHECK(other);
                natural tmp(other); xch(tmp);
                Y_SIBYL_NATURAL_CHECK(*this);
                return *this;
            }
            
            //! assign by copy/xch
            inline natural & operator=(const utype _)
            {
                natural tmp(_); xch(tmp);
                Y_SIBYL_NATURAL_CHECK(*this);
                return *this;
            }
            

            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! set to zero
            inline void ldz() throw()
            {
                Y_SIBYL_NATURAL_CHECK(*this);
                memset(word,0,width);
                bytes=0;
                words=0;
                Y_SIBYL_NATURAL_CHECK(*this);
            }

            //! no-throw exchange
            inline void xch( natural &other) throw()
            {
                Y_SIBYL_NATURAL_CHECK(other);
                Y_SIBYL_NATURAL_CHECK(*this);
                cswap(bytes,other.bytes);
                cswap(count,other.count);
                cswap(width,other.width);
                cswap(shift,other.shift);
                cswap(words,other.words);
                cswap(word, other.word );
                Y_SIBYL_NATURAL_CHECK(other);
                Y_SIBYL_NATURAL_CHECK(*this);
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

            inline void  display(std::ostream &os) const
            {
                Y_SIBYL_NATURAL_CHECK(*this);
                for(size_t i=bytes;i>0;)
                {
                    --i;
                    os<< hexadecimal::lowercase[ get(i) ];
                }
            }

            inline friend std::ostream & operator<<( std::ostream &os, const natural &n )
            {
                n.display(os);
                return os;
            }

            //! get least significant words
            utype lsw() const throw()
            {
                Y_SIBYL_NATURAL_CHECK(*this);
                utype ans = 0;
                for(size_t i=words_per_utype;i>0;)
                {
                    ans <<= word_bits;
                    ans |=  utype(word[--i]);
                }
                return ans;
            }

#define Y_SIBYL_NATURAL_U2W(ARGS) size_t nw = 0; const word_type *pw=u2w(ARGS,nw)

#define Y_SIBYL_NATURAL_COMPLETE(FUNCTION) \
static inline natural FUNCTION(const natural &lhs, const natural &rhs) { return FUNCTION(lhs.word,lhs.words,rhs.word,rhs.words);            }\
static inline natural FUNCTION(const natural &lhs, utype          u  ) { Y_SIBYL_NATURAL_U2W(u); return FUNCTION(lhs.word,lhs.words,pw,nw); }\
static inline natural FUNCTION(utype          u,   const natural &rhs) { Y_SIBYL_NATURAL_U2W(u); return FUNCTION(pw,nw,rhs.word,rhs.words); }

#define Y_SIBYL_NATURAL_NO_THROW(RETURN,FUNCTION) \
static inline RETURN FUNCTION(const natural &lhs, const natural &rhs) throw() { return FUNCTION(lhs.word,lhs.words,rhs.word,rhs.words);            }\
static inline RETURN FUNCTION(const natural &lhs, utype          u  ) throw() { Y_SIBYL_NATURAL_U2W(u); return FUNCTION(lhs.word,lhs.words,pw,nw); }\
static inline RETURN FUNCTION(utype          u,   const natural &rhs) throw() { Y_SIBYL_NATURAL_U2W(u); return FUNCTION(pw,nw,rhs.word,rhs.words); }


            //__________________________________________________________________
            //
            // addition
            //__________________________________________________________________
            Y_SIBYL_NATURAL_COMPLETE(add)


            //__________________________________________________________________
            //
            // equality
            //__________________________________________________________________
            Y_SIBYL_NATURAL_NO_THROW(bool,eq)


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

            //! utype -> words, and number of words = words_per_utype
            static inline const word_type *u2w( utype &u, size_t &n ) throw()
            {
                u            = swap_le(u);
                word_type *w = (word_type *)&u;
                n            = words_per_utype;
                size_t m     = words_per_utype-1;
                while(n>0&&w[m]<=0)
                {
                    --n;
                    --n;
                }
                return w;
            }

        private:
            size_t     bytes; //!< active: number of bytes
            size_t     words; //!< active: number of words
            size_t     count; //!< memory: max number of words
            size_t     width; //!< memory: width = count * sizeof(word_type), max number of bytes
            size_t     shift; //!< memory: width = 1 << shift
            word_type *word;  //!< address

            //! decrease bytes to first not 0
            inline void update() throw()
            {
                size_t curr = bytes;
                size_t prev = curr-1;
                while(curr>0&&get(prev)<=0)
                    curr = prev--;
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
                natural      ans(num,as_capacity);
                word_type   *sum   = ans.word;
                core_type    carry = 0;

                //--------------------------------------------------------------
                // common part
                //--------------------------------------------------------------
                for(size_t i=0;i<small_size;++i)
                {
                    carry += core_type(small_data[i])+core_type(large_data[i]);
                    sum[i] = word_type(carry);
                    carry >>= word_bits;
                    assert(carry<core_type(max_word));
                }

                //--------------------------------------------------------------
                // propagate carry
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

                ans.bytes = num;
                ans.update();
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


        };

    }

}


#endif

