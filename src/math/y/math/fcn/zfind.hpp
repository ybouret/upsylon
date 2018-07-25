//! \file
#ifndef Y_MATH_ZFIND_INCLUDED
#define Y_MATH_ZFIND_INCLUDED 1

#include "y/math/types.hpp"
#include "y/math/triplet.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace math
    {

        //! finding zero
        struct zfind
        {
            static const unsigned __z  = 0x00; //!< zero
            static const unsigned __p  = 0x01; //!< positive
            static const unsigned __n  = 0x02; //!< negative

            static const unsigned _zz  = (__z<<8)|__z; //!< zero|zero

            static const unsigned _zp  = (__z<<8)|__p; //!< zero|positive
            static const unsigned _pz  = (__p<<8)|__z; //!< positive|zero

            static const unsigned _zn  = (__z<<8)|__n; //!< zero|negative
            static const unsigned _nz  = (__n<<8)|__z; //!< negative|zero

            static const unsigned _np  = (__n<<8)|__p; //!< negative|positive
            static const unsigned _pn  = (__p<<8)|__n; //!< positive|negative

            //! get the sign of x
            template <typename T>
            static inline unsigned __sign(const T x) throw()
            {
                return (x<0) ? __n : ( (0<x) ? __p : __z);
            }

            //! find zero with precomputed triplets
            template <typename T,typename FUNC>
            static inline
            bool run( FUNC &F, triplet<T> &x, triplet<T> &f )
            {

                T w = __fabs(x.c-x.a);
                while(true)
                {
                    f.b = F( (x.b = T(0.5)*(x.a+x.c) ) );
                    const unsigned ab     = __sign( f.a*f.b );
                    const unsigned bc     = __sign( f.b*f.c );
                    const unsigned status = (ab<<8) | bc;

                    switch(status)
                    {
                        case _zz: // => f.b = 0
                            x.ld(x.b);
                            f.ld(f.b);
                            return true;
                        case _zn: /* FALLTHRU */
                        case _zp: // => f.a = 0
                            x.ld(x.a);
                            f.ld(f.a);
                            return true;

                        case _nz: /* FALLTHRU */
                        case _pz: // => f.c = 0
                            x.ld(x.c);
                            f.ld(f.c);
                            return true;

                        case _np: // f.b same sign than f.c
                            x.c = x.b;
                            f.c = f.b;
                            break;

                        case _pn: // f.b same sign than f.a
                            x.a = x.b;
                            f.a = f.b;
                            break;

                        default:
                            return false;
                    }

                    const T new_w = __fabs(x.c-x.a);
                    if(new_w>=w) return true;
                    w = new_w;
                }
            }

#if 0
            template <typename T,typename FUNC>
            struct zcall
            {
                FUNC *pF;
                T     F0;
                inline T operator()(const T x ) { assert(pF); return (*pF)(x)-F0; }
            };
#endif

        };

    }
}

#endif

