 
//! \file

#ifndef Y_TYPE_MEMOPS_INCLUDED
#define Y_TYPE_MEMOPS_INCLUDED 1

#include "y/code/ilog2.hpp"
#include "y/type/select.hpp"
#include "y/type/ints.hpp"

namespace upsylon {

    namespace core
    {

        //! memory alignement choice
        template <size_t LENGTH,size_t ILN2>
        struct memcull
        {
            static  const    size_t                              guess_word_size = (1<<ILN2); //!< 2^ILN2
            typedef typename unsigned_int<guess_word_size>::type guess_word_type;             //!< unsigned matching typw
            typedef typename select_type<0==(LENGTH%guess_word_size),
            guess_word_type,
            typename memcull<LENGTH,ILN2-1>::word_type
            >::result word_type; //!< meta-selected type
        };

        //! default word type
        template <size_t LENGTH>
        struct memcull<LENGTH,0>
        {
            typedef uint8_t word_type; //!< for unaligned lengths
        };

        //! runing operations on blocks
        template <typename T, const size_t N>
        struct memrun
        {
            //! zero T[N]
            static inline void zero( void *addr ) throw()
            {
                T *p = static_cast<T*>(addr);
                for(size_t i=0;i<N;++i) p[i] = 0;
            }

            //! copy T[N]
            static inline void copy( void *target, const void *source ) throw()
            {
                T       *tgt = static_cast<T*>(target);
                const T *src = static_cast<const T*>(source);
                for(size_t i=0;i<N;++i) tgt[i] = src[i];
            }

            //! swap T[N]
            static inline void swap( void *target, void *source ) throw()
            {
                T *tgt = static_cast<T*>(target);
                T *src = static_cast<T*>(source);
                for(size_t i=0;i<N;++i)
                {
                    T    &  t   = tgt[i];
                    T    &  s   = src[i];
                    const T tmp = t; t=s; s=tmp;
                }
            }
        };

        //! special cases for 1-block
        template <typename T>
        struct memrun<T,1>
        {
            //! zero one block
            static inline void zero( void *addr ) throw()
            {
                *static_cast<T*>(addr) = 0;
            }

            //! copy one block
            static inline void copy( void *target, const void *source ) throw()
            {
                *static_cast<T*>(target) = *static_cast<const T*>(source);
            }

            //! swap one block
            static inline void swap( void *target, void *source ) throw()
            {
                T &t = *static_cast<T*>(target);
                T &s = *static_cast<T*>(source);
                const T tmp = t; t=s; s=tmp;
            }
        };


        //! computing meta data for memory operations
        template <size_t LENGTH>
        struct memops
        {
            //typedef ptrdiff_t                    default_type;
            typedef uint32_t                     default_type;                                 //!< maximum default aligned data type
            static  const size_t                 default_iln2 = ilog2_of<default_type>::value; //!< its integer ln2
            typedef memcull<LENGTH,default_iln2> culled;                                       //!< type selector
            typedef typename culled::word_type   word_type;                                    //!< resulting word type
            static  const size_t                 word_size = sizeof(word_type);                //!< word_size
            static  const size_t                 type_size = LENGTH;                           //!< alias
            static  const size_t                 num_words = type_size/word_size;              //!< number of blocks
        };

    }

    //! memory BITWISE operations
    struct memops
    {
        //! a=0
        template <typename T> static inline void zero(T &a) throw()
        {
            typedef core::memops<sizeof(T)> ops;
            typedef typename ops::word_type word_type;
            assert( sizeof(word_type) * ops::num_words == sizeof(T) );
            core::memrun<word_type,ops::num_words>::zero(&a);
        }

        //! force a=0
        template <typename T> static inline void zero_(const T &a) throw()
        {
            zero((T&)a);
        }

        //! a=b
        template <typename T> static inline void copy(T &a, const T &b) throw()
        {
            typedef core::memops<sizeof(T)> ops;
            typedef typename ops::word_type word_type;
            assert( sizeof(word_type) * ops::num_words == sizeof(T) );
            core::memrun<word_type,ops::num_words>::copy(&a,&b);
        }

        //! a <-> b
        template <typename T> static inline void swap(T &a, T &b) throw()
        {
            typedef core::memops<sizeof(T)> ops;
            typedef typename ops::word_type word_type;
            assert( sizeof(word_type) * ops::num_words == sizeof(T) );
            core::memrun<word_type,ops::num_words>::swap(&a,&b);
        }

        //! force a <-> b
        template <typename T> static inline void swap_(const T &a, const T &b) throw()
        {
            typedef core::memops<sizeof(T)> ops;
            typedef typename ops::word_type word_type;
            assert( sizeof(word_type) * ops::num_words == sizeof(T) );
            core::memrun<word_type,ops::num_words>::swap((void*)&a,(void*)&b);
        }
        

    };



}

#endif

