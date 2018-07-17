//! \file
#ifndef Y_MATH_TAO_INCLUDED
#define Y_MATH_TAO_INCLUDED 1

#include "y/container/matrix.hpp"
#include "y/sequence/array.hpp"
#include "y/concurrent/scheme/simd.hpp"

namespace upsylon
{
    namespace math
    {

        struct tao
        {
            ////////////////////////////////////////////////////////////////////
            //
            // level-1 : array ops
            //
            ////////////////////////////////////////////////////////////////////
            template <typename T>
            static void _ld( array<T> &a, typename type_traits<T>::parameter_type v )
            {
                for(size_t i=a.size();i>0;--i)
                {
                    a[i] = v;
                }
            }

            template <typename T> struct __ld
            {
                Y_DECL_ARGS(T,type);
                array<type> *a;
                const_type  *v;
                static inline void call(void *args, parallel &ctx, lockable &)
                {
                    __ld        *self = (__ld *)args;
                    array<type> &a    = *(self->a);
                    const_type  &v    = *(self->v);
                    size_t offset = 1;
                    size_t length = a.size();
                    ctx.split(length,offset);
                    while(length--)
                    {
                        a[offset++] = v;
                    }
                }
            };

            template <typename T>
            static void _ld( array<T> &a, typename type_traits<T>::parameter_type v, concurrent::for_each &loop )
            {
                __ld<T> args = { &a, &v };
                loop.run( __ld<T>::call, &args );
            }

            template <typename T>
            static void ld( array<T> &a, typename type_traits<T>::parameter_type v, concurrent::for_each *loop )
            {
                if(loop)
                {
                    _ld(a,v,*loop);
                }
                else
                {
                    _ld(a,v);
                }
            }

        };

    }
}

#endif

