//! \file

#ifndef Y_GFX_PIXEL_INTERPOLATION_INCLUDED
#define Y_GFX_PIXEL_INTERPOLATION_INCLUDED 1

#include "y/gfx/pixel/blend.hpp"
#include "y/code/hunt.hpp"

namespace upsylon
{
    namespace graphic
    {

        //______________________________________________________________________
        //
        //
        //! functions to interpolate pixels
        //
        //______________________________________________________________________
        struct interp
        {
            //! x[0..N-1] from 0<=xx<=1
            template <typename T, const unsigned N> static inline
            T closest_(const float xx, const T arr[]) throw()
            {
                assert(xx>=0.0f);
                assert(xx<=1.0f);
                assert(NULL!=arr);
                return arr[ unsigned( floorf(xx*(N-1)+0.5f) ) ];
            }

            //! x[0..num-1] from 0<=xx<<1
            template <typename T> static inline
            T closest_(const float xx, const T arr[], size_t num) throw()
            {
                assert(xx>=0.0f);
                assert(xx<=1.0f);
                assert(NULL!=arr);
                return arr[ size_t( floorf(--num*xx) + 0.5f) ];
            }

            //! arr[0..N-1] from 0<=xx<=1 with alpha
            template <typename T, const unsigned N> static inline
            T linear_(const float xx, const T arr[]) throw()
            {
                const float    xs    = xx * (N-1);
                unsigned       jm    = unsigned( floorf(xs+0.5f) );
                if(jm>=N-1)    jm    = N-2;
                const unsigned jp    = jm+1;
                const float    alpha = jp-xs;
                return blend<float,T>::mix(alpha,arr[jm],arr[jp]);
            }

            //! arr[0..num-1] from 0<=xx<=1 with alpha
            template <typename T> static inline
            T linear_(const float xx, const T arr[], size_t num) throw()
            {
                const float    xs    = xx * --num;
                size_t         jm    = size_t( floorf(xs+0.5f) );
                if(jm>=num)    jm    = --num;
                const size_t   jp    = jm+1;
                const float    alpha = float(jp)-xs;
                return blend<float,T>::mix(alpha,arr[jm],arr[jp]);
            }

            //! y[1..num] from 0<=xx<=1 with alpha
            template <
            typename T,
            typename ORDINATE> static inline
            T linear(const float xx, ORDINATE &y, size_t n) throw()
            {
                assert(xx>=0.0f);
                assert(xx<=1.0f);
                assert(n>1);
                const float xs    = xx * --n;
                size_t      jm    = size_t(floorf(xs+0.5f));
                if(jm>=n)   jm    = --n;
                size_t      jp    = jm+1;
                const float alpha = float(jp)-xs;
                return blend<float,T>::mix(alpha,y[jm+1],y[jp+1]);
            }

            template <typename SEQUENCE> static inline
            typename SEQUENCE::type linear(const float xx, const SEQUENCE &seq) throw()
            {
                return linear<typename SEQUENCE::mutable_type,const SEQUENCE>(xx,seq,seq.size());
            }


            //! linear interpolation xx in x[1..n] and y[1..n], with starting point j
            template <
            typename T,
            typename ABSCISSA,
            typename ORDINATE> static inline
            T linear(const float xx, ABSCISSA &x, ORDINATE &y, const size_t n, size_t &j) throw()
            {
                assert(n>0);
                switch( hunt::search(xx,x,n,j) )
                {
                    case hunt::found_below: return y[1];
                    case hunt::found_above: return y[n];
                    case hunt::found_inner:
                        break;
                }
                const size_t jm    = j;
                const size_t jp    = j+1;
                const float  xm    = x[jm];
                const float  xp    = x[jp];
                const float  dx    = xp-xm;
                const float  alpha = (xp-xx)/dx;
                return blend<float,T>::mix(alpha,y[jm],y[jp]);
            }

        };

    }

}

#endif
