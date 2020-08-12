
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


        template <size_t WORD_BITS>
        class natural : public number
        {
        public:
            static const size_t                            word_bits = Y_ROUND8(WORD_BITS);
            static const size_t                            word_size = word_bits >> 3;
            static const size_t                            word_exp2 = ilog2<word_size>::value;
            static const size_t                            word_mask = word_size-1;
            typedef typename unsigned_int<word_size>::type word_type;
            static const word_type                         max_word      = limit_of<word_type>::maximum;
            static  const size_t                           min_core_size = word_size << 1;
            static  const size_t                           core_size     = (min_core_size>sys_core_size) ? max_core_size : sys_core_size;
            static  const size_t                           find_size     = (core_size>=min_core_size) ? core_size : 0;
            typedef typename unsigned_int<find_size>::type core_type;
            typedef uint64_t                               utype;


            static  inline size_t     words_for( const size_t bytes ) throw() { return Y_ALIGN_FOR_ITEM(word_type,bytes) >> word_exp2;  }
            static  inline word_type *acquire(size_t &count, size_t &width, size_t &shift)
            {
                static memory_allocator &mgr = instance();
                return mgr.acquire_field<word_type>(count,width,shift);
            }

            static inline void release(word_type * &w, size_t &count, size_t &width, size_t &shift) throw()
            {
                static memory_allocator &mgr = location();
                mgr.release_field(w,count,width,shift);
            }


            size_t     bytes; //!< number of bytes
            size_t     count; //!< number of words
            size_t     width; //!< memory size = count * sizeof(word_type)
            size_t     shift; //!< _size = 1 << _exp2
            word_type *words; //!< active memory

            inline void ldz() throw()
            {
                memset(words,0,width);
                bytes=0;
            }

            inline virtual ~natural() throw()
            {
                release(words,count,width, shift);
            }

            inline explicit natural() : Y_SIBYL_NATURAL_CTOR(0) {}

            inline explicit natural(const size_t n, const as_capacity_t &) : Y_SIBYL_NATURAL_CTOR(n) {}

            inline void xch( natural &_ ) throw()
            {
                cswap(bytes,_.bytes);
                cswap(count,_.count);
                cswap(width,_.width);
                cswap(shift,_.shift);
                cswap(words,_.words);
            }

            inline explicit natural(utype u) : Y_SIBYL_NATURAL_CTOR(sizeof(utype))
            {
                static const size_t nw = sizeof(utype)>>word_exp2;
                std::cerr << "copy #words=" << nw << std::endl;
                for(size_t iw=0;iw<nw;++iw)
                {
                    words[iw] = word_type(u);
                    u >>= word_bits;
                }
            }

            //! access all addressable bytes
            uint8_t &get(const size_t indx) const throw()
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
                for(size_t i=width;i>0;)
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






        private:
            Y_DISABLE_COPY_AND_ASSIGN(natural);
        };

    }

}


#endif

