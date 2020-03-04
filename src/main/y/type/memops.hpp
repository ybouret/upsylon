 
//! \file

#ifndef Y_TYPE_MEMOPS_INCLUDED
#define Y_TYPE_MEMOPS_INCLUDED 1

#include "y/code/ilog2.hpp"
#include "y/type/select.hpp"
#include "y/type/ints.hpp"

namespace upsylon {

    namespace core
    {

        template <size_t LENGTH,size_t ILN2>
        struct memcull
        {
            static  const    size_t                              guess_word_size = (1<<ILN2);
            typedef typename unsigned_int<guess_word_size>::type guess_word_type;
            typedef typename select_type<0==(LENGTH%guess_word_size),
            guess_word_type,
            typename memcull<LENGTH,ILN2-1>::word_type
            >::result word_type;
        };

        template <size_t LENGTH>
        struct memcull<LENGTH,0>
        {
            typedef uint8_t word_type;
        };

        template <size_t LENGTH>
        struct memops
        {
            typedef ptrdiff_t                    default_type;
            static  const size_t                 default_iln2 = ilog2_of<default_type>::value;
            typedef memcull<LENGTH,default_iln2> culled;
            typedef typename culled::word_type   word_type;
            static  const size_t                 word_size = sizeof(word_type);
            static  const size_t                 type_size = LENGTH;
            static  const size_t                 num_words = type_size/word_size;
        };

    }

    struct memops
    {
        template <typename T> static inline
        void zero( T &a ) throw()
        {
            typedef core::memops<sizeof(T)> ops;
            typedef typename ops::word_type word_type;
            word_type *A = (word_type *) &a;
            for(size_t i=0;i<ops::num_words;++i)
            {
                A[i] = 0;
            }
        }
    };



}

#endif

