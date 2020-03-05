//! \file

#ifndef Y_TYPE_BLOCK_ZSET_INCLUDED
#define Y_TYPE_BLOCK_ZSET_INCLUDED 1

#include "y/type/cull.hpp"

namespace upsylon {

    namespace core {

        //! meta block_zset run
        template <typename T, const size_t N>
        struct block_zset
        {
            //! call on first block and meta-recursive next one
            static inline void on(void *addr) throw()
            {
                T *target = static_cast<T *>(addr);
                target[0] = 0;
                block_zset<T,N-1>::on(target+1);
            }
        };

        //! meta block_zset end
        template <typename T>
        struct block_zset<T,1>
        {
            //! zero current block
            static inline void on(void *addr) throw()
            {
                *static_cast<T*>(addr) = 0;
            }
        };

    }

    //! memset(addr,0,LENGTH)
    template <const size_t LENGTH>
    inline void block_zset(void *addr) throw()
    {
        typedef cull<LENGTH>             info;
        typedef typename info::word_type word_type;
        assert( sizeof(word_type) * info::num_words == LENGTH );

        core::block_zset<word_type,info::num_words>::on(addr);
    }

    //! memset(&args,0,sizeof(T))
    template <typename T>
    inline void bzset(T &args) throw()
    {
        typedef cull<sizeof(T)>          info;
        typedef typename info::word_type word_type;
        assert( sizeof(word_type) * info::num_words == sizeof(T) );

        core::block_zset<word_type,info::num_words>::on(&args);
    }

    //! forced bzset
    template <typename T>
    inline void _bzset(const T &args) throw()
    {
        bzset( (T&)args );
    }

    //! bzset blocks like 'TYPE BLK[N];'
#define Y_BZSET_STATIC(BLK) block_zset<sizeof(BLK)>(BLK)

    //! test zeroed block
    template <const size_t LENGTH>
    inline bool block_is_zeroed(const void *addr) throw()
    {
        const uint8_t *byte = static_cast<const uint8_t *>(addr);
        for(size_t i=0;i<LENGTH;++i)
        {
            if( 0 != byte[i] ) return false;
        }
        return true;
    }

    //! test zeroed args
    template <typename T>
    inline bool is_zeroed(const T &args) throw()
    {
        return block_is_zeroed<sizeof(T)>(&args);
    }

}


#endif

