//! \file

#ifndef Y_TYPE_BCOPY_INCLUDED
#define Y_TYPE_BCOPY_INCLUDED 1

#include "y/core/loop.hpp"
#include <cstring>

namespace upsylon {
 
    namespace core
    {
#define Y_BCOPY_CHECK()             \
assert( !(0==target&&numBlocks>0)); \
assert( !(0==source&&numBlocks>0))
        
        template <size_t BLOCK_SIZE>
        inline void bcopy( void *target, const void *source, const size_t numBlocks ) throw()
        {
            Y_BCOPY_CHECK();
            memcpy(target,source,numBlocks*BLOCK_SIZE);
        }
        
        template <>
        inline void bcopy<1>(void *target, const void *source, const size_t numBlocks ) throw()
        {
            Y_BCOPY_CHECK();
            memcpy(target,source,numBlocks);
        }
        
#define Y_BCOPY(I) t[I] = s[I]
        template <>
        inline void bcopy<2>(void *target, const void *source, const size_t numBlocks ) throw()
        {
            Y_BCOPY_CHECK();
            uint16_t       *t = static_cast<uint16_t       *>( target );
            const uint16_t *s = static_cast<const uint16_t *>( source );
            Y_LOOP_FUNC(numBlocks,Y_BCOPY,0);
        }
        
        template <>
        inline void bcopy<4>(void *target, const void *source, const size_t numBlocks ) throw()
        {
            Y_BCOPY_CHECK();
            uint32_t       *t = static_cast<uint32_t       *>( target );
            const uint32_t *s = static_cast<const uint32_t *>( source );
            Y_LOOP_FUNC(numBlocks,Y_BCOPY,0);
        }
        
        template <>
        inline void bcopy<8>(void *target, const void *source, const size_t numBlocks ) throw()
        {
            Y_BCOPY_CHECK();
            uint64_t       *t = static_cast<uint64_t       *>( target );
            const uint64_t *s = static_cast<const uint64_t *>( source );
            Y_LOOP_FUNC(numBlocks,Y_BCOPY,0);
        }
    }
    
    template <typename T>
    inline void bcopy( T *target, const T *source, const size_t numBlocks) throw()
    {
        core::bcopy<sizeof(T)>(target, source, numBlocks);
    }
    
}

#endif

