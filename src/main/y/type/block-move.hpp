
//! \file

#ifndef Y_TYPE_BLOCK_MOVE_INCLUDED
#define Y_TYPE_BLOCK_MOVE_INCLUDED 1

#include "y/type/cull.hpp"

namespace upsylon {

    namespace core {

        //! meta block_move run
        template <typename T, const size_t N>
        struct block_move
        {
            //! call on first block and meta-recursive next one
            static inline void on(void *target, const void *source) throw()
            {
                T       *t = static_cast<T       *>(target);
                const T *s = static_cast<const T *>(source);
                t[0] = s[0];
                block_move<T,N-1>::on(t+1,s+1);
            }
        };

        //! meta block_move end
        template <typename T>
        struct block_move<T,1>
        {
            //! copy current block
            static inline void on(void *target, const void *source) throw()
            {
                *(T*)target = *(const T*)source;
            }
        };

    }

    //! memmove(target,source,LENGTH)
    template <const size_t LENGTH>
    inline void block_move(void *target,const void *source) throw()
    {
        typedef cull<LENGTH>             info;
        typedef typename info::word_type word_type;
        assert( sizeof(word_type) * info::num_words == LENGTH );

        core::block_move<word_type,info::num_words>::on(target,source);
    }

    //! memmove( &target, &source, sizeof(T) )
    template <typename T>
    inline void bmove(T &target, const T &source) throw()
    {
        typedef cull<sizeof(T)>          info;
        typedef typename info::word_type word_type;
        assert( sizeof(word_type) * info::num_words == sizeof(T) );

        core::block_move<word_type,info::num_words>::on(&target,&source);
    }



}


#endif

