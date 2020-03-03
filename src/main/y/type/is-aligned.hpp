//! \file

#ifndef Y_TYPE_IS_ALIGNED_INCLUDED
#define Y_TYPE_IS_ALIGNED_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {


#define Y_IS_ALIGNED_ON16_OR_ON8(LENGTH)  ( 0==(LENGTH%2) ? is_aligned::on16 : is_aligned::on8 )
#define Y_IS_ALIGNED_ON32_OR_LESS(LENGTH) ( 0==(LENGTH%4) ? is_aligned::on32 : Y_IS_ALIGNED_ON16_OR_ON8(LENGTH) )
#define Y_IS_ALIGNED_VALUE(LENGTH)        ( 0==(LENGTH%8) ? is_aligned::on64 : Y_IS_ALIGNED_ON32_OR_LESS(LENGTH) )

    struct is_aligned
    {
        enum value
        {
            on8,   //!< on  8-bits|1-byte  -> any data
            on16,  //!< on 16-bits|2-bytes -> multiple of uint16_t
            on32,  //!< on 32-bits|4-bytes -> multiple of uint32_t
            on64   //!< on 64-bits|8-bytes -> multiple of uint64_t
        };

        static const char *text( const value )  throw();

        template <value ON> struct type_for;

    };

    namespace core {

        template <typename T,size_t N> struct memops
        {
            static inline void bcopy( void *target, const void *source ) throw()
            {
                T       *tgt = static_cast<T       *>(target);
                const T *src = static_cast<const T *>(source);
                for(size_t i=0;i<N;++i) tgt[i] = src[i];
            }

            static inline void bswap( void *target, void *source ) throw()
            {
                T *tgt = static_cast<T *>(target);
                T *src = static_cast<T *>(source);
                for(size_t i=0;i<N;++i) { const T tmp = tgt[i]; tgt[i] = src[i]; src[i] = tmp; }
            }
        };

        template <typename T> struct memops<T,1>
        {
            static inline void bcopy( void *target, const void *source ) throw()
            {
                *(T*)target = *(const T*)source;
            }

            static inline void bswap( void *target, void *source ) throw()
            {
                T &tgt = *(T*)target;
                T &src = *(T*)source;
                const T tmp = tgt; tgt=src; src=tmp;
            }
        };


    }



#define Y_IS_ALIGNED_ON(BITS) \
template <> struct is_aligned:: type_for<is_aligned::on##BITS> { \
typedef uint##BITS##_t    word_type;\
static inline value       kind() throw() { return is_aligned::on##BITS;       }\
static inline const char *text() throw() { return is_aligned::text( kind() ); }\
}


    Y_IS_ALIGNED_ON(8);
    Y_IS_ALIGNED_ON(16);
    Y_IS_ALIGNED_ON(32);
    Y_IS_ALIGNED_ON(64);

#define Y_ALIGNED_TYPE_FOR(LENGTH) is_aligned::type_for<Y_IS_ALIGNED_VALUE(LENGTH)>

    template <typename T>
    struct aligned_type_for
    {
        static const size_t                   type_size = sizeof(T);
        typedef Y_ALIGNED_TYPE_FOR(type_size) context;
        typedef typename context::word_type   word_type;
        static const size_t                   word_size = sizeof(word_type);
        static const size_t                   words     = type_size / word_size;
        static inline const char *text() throw() { return context::text(); }
    };

    template <typename T>
    void  bswap( T &target, T &source ) throw()
    {
        typedef aligned_type_for<T> _;
        core::memops<typename _::word_type,_::words>::bswap( &target, &source );
    }




}

#endif
