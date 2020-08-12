
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
#include "y/sequence/addressable.hpp"

namespace upsylon {


    namespace sibyl {

#define Y_SIBYL_NATURAL_CTOR(BYTES) \
number(), \
bytes(BYTES),                  \
count( words_for(bytes) ),\
width(0),\
shift(0),\
words( acquire(count,width,shift) ) 

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
            //! try to use system core type, otherwise fallback on largest type
            static  const size_t                           core_size     = (min_core_size>sys_core_size) ? max_core_size : sys_core_size;
            //! validate size
            static  const size_t                           find_size     = (core_size>=min_core_size) ? core_size : 0;
            typedef typename unsigned_int<find_size>::type core_type; //!< internal core_type
            typedef uint64_t                               utype;     //!< unsigned integral type

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! cleanup
            inline virtual ~natural() throw()
            {
                release(words,count,width, shift);
            }

            //! default constructor: 0
            inline  natural() : Y_SIBYL_NATURAL_CTOR(0) {}

            //! default constructor with some capacity
            inline  natural(const size_t n, const as_capacity_t &) : Y_SIBYL_NATURAL_CTOR(n) {}

            //! construct from an integral type
            inline natural(utype u) : Y_SIBYL_NATURAL_CTOR(sizeof(utype))
            {
                static const size_t nw = sizeof(utype)>>word_exp2;
                for(size_t iw=0;iw<nw;++iw)
                {
                    words[iw] = word_type(u);
                    u >>= word_bits;
                }
                update();
            }

            //! copy constructor
            inline natural(const natural &_) : Y_SIBYL_NATURAL_CTOR(_.bytes)
            {
                memcpy(words,_.words,bytes);
            }

#define Y_SIBYL_NATURAL_ASSIGN(ARGS) \
inline natural & operator=(ARGS _) \
{\
/**/ natural tmp(_); xch(tmp); return *this;\
}

            Y_SIBYL_NATURAL_ASSIGN(const natural &)
            Y_SIBYL_NATURAL_ASSIGN(const utype)


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! set to zero
            inline void ldz() throw()
            {
                memset(words,0,width);
                bytes=0;
            }

            //! no-throw exchange
            inline void xch( natural &_ ) throw()
            {
                cswap(bytes,_.bytes);
                cswap(count,_.count);
                cswap(width,_.width);
                cswap(shift,_.shift);
                cswap(words,_.words);
            }


            //! access all addressable bytes in 0..width-1
            inline uint8_t &get(const size_t indx) const throw()
            {
                assert(indx<width);
                const word_type &w = words[ indx >> word_exp2];
                uint8_t         *p = (uint8_t *)&w;
                const size_t     i = indx &  word_mask;
#if Y_BYTE_ORDER == Y_BIG_ENDIAN
                return p[word_mask-i];
#else
                return p[i];
#endif
            }

            inline void  display(std::ostream &os ) const
            {
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


        private:
            size_t     bytes; //!< number of bytes
            size_t     count; //!< number of words
            size_t     width; //!< memory size = count * sizeof(word_type)
            size_t     shift; //!< _size = 1 << _exp2
            word_type *words; //!< active memory

            //! decrease bytes to first not 0
            inline void update() throw()
            {
                size_t curr = bytes;
                size_t prev = curr-1;
                while(curr>0&&get(prev)<=0)
                    curr = prev--;
                bytes = curr;
            }

            //! set bytes to width and update
            inline void upgrade() throw()
            {
                bytes=width;
                update();
            }
        };

    }

}


#endif

