//! \file
#ifndef Y_TYPE_CULL_INCLUDED
#define Y_TYPE_CULL_INCLUDED 1

#include "y/code/ilog2.hpp"
#include "y/type/ints.hpp"

namespace upsylon {

    namespace core {

        //! memory alignement choice
        template <size_t LOG2,size_t LENGTH>
        struct cull_on
        {
            static  const    size_t                              guess_word_size = (1<<LOG2);                    //!< 2^ILN2
            typedef typename unsigned_int<guess_word_size>::type guess_word_type;                                //!< unsigned matching type
            static  const    bool                                is_aligned_with = (0==(LENGTH%guess_word_size));//!< check LENGTH is multiple of guess
            typedef typename select_type<is_aligned_with,
            guess_word_type,
            typename cull_on<LOG2-1,LENGTH>::word_type
            >::result word_type; //!< meta-selected type
        };

        //! default word type => byte
        template <size_t LENGTH>
        struct cull_on<0,LENGTH>
        {
            typedef uint8_t word_type; //!< for unaligned lengths
        };



    }

    //! computing meta data for memory operations
    template <size_t LENGTH>
    struct cull
    {
        typedef ptrdiff_t                     os_type;                                      //!< maximum default aligned data type
        static  const size_t                  os_log2 = ilog2_of<os_type>::value;           //!< its integer log2
        typedef core::cull_on<os_log2,LENGTH> culled;                                       //!< type selector
        typedef typename culled::word_type    word_type;                                    //!< resulting word type
        static  const size_t                  word_size = sizeof(word_type);                //!< word_size
        static  const size_t                  type_size = LENGTH;                           //!< alias
        static  const size_t                  num_words = type_size/word_size;              //!< number of blocks
    };

    //! testing bytewise same blocks
    template <size_t LENGTH>
    inline bool are_same_blocks( const void *a, const void *b ) throw()
    {
        assert(a);
        assert(b);
        const uint8_t *A = static_cast<const uint8_t *>(a);
        const uint8_t *B = static_cast<const uint8_t *>(b);
        for(size_t i=0;i<LENGTH;++i)
        {
            if(A[i]!=B[i]) return false;
        }
        return true;
    }

    //! testing bytewise same objects
    template <typename T>
    inline bool bsame( const T &a, const T &b ) throw()
    {
        return are_same_blocks<sizeof(T)>(&a,&b);
    }
}

#endif

