
//! \file

#ifndef Y_TYPE_BLOCK_SWAP_INCLUDED
#define Y_TYPE_BLOCK_SWAP_INCLUDED 1

#include "y/type/cull.hpp"

namespace upsylon {

    namespace core {

#define Y_BLOCK_SWAP_EXEC() \
T &t = static_cast<T *>(target);\
T &s = static_cast<T *>(source);\
const T tmp = t; t=s; s=tmp

        //! meta block_swap run
        template <typename T, const size_t N>
        struct block_swap
        {
            //! call on first block and meta-recursive next one
            static inline void on(void *target, void *source) throw()
            {
                Y_BLOCK_SWAP_EXEC();
                block_swap<T,N-1>::on(&t+1,&s+1);
            }
        };

        //! meta block_swap end
        template <typename T>
        struct block_swap<T,1>
        {
            //! zero current block
            static inline void on(void *target, void *source) throw()
            {
                Y_BLOCK_SWAP_EXEC();
            }
        };

    }

    //! swap blocks
    template <const size_t LENGTH>
    inline void block_swap(void *target, void *source) throw()
    {
        typedef cull<LENGTH>             info;
        typedef typename info::word_type word_type;
        assert( sizeof(word_type) * info::num_words == LENGTH );

        core::block_swap<word_type,info::num_words>::on(target,source);
    }

    //! swap blocks
    template <typename T>
    inline void bswap(T &target, T &source) throw()
    {
        typedef cull<sizeof(T)>          info;
        typedef typename info::word_type word_type;
        assert( sizeof(word_type) * info::num_words == sizeof(T) );

        core::block_swap<word_type,info::num_words>::on(&target,&source);
    }



}


#endif

