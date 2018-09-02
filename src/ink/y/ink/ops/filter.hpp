//! \file
#ifndef Y_INK_OPS_FILTER_INCLUDED
#define Y_INK_OPS_FILTER_INCLUDED 1

#include "y/ink/pixmap.hpp"
#include "y/ink/pixel.hpp"
#include "y/container/matrix.hpp"

namespace upsylon
{
    namespace Ink
    {

        //! to apply function on pixmaps
        class Filter
        {
        public:
#include "filter/apply.hxx"
            //! target = func(source), with an engine
            template <typename T,typename U,typename FUNC> static inline
            void Apply(Pixmap<T>       &target,
                       const Pixmap<U> &source,
                       FUNC            &func,
                       Engine          &E)
            {
                __apply<T,U,FUNC> proxy = { &target, &source, &func };
                E.run(proxy);
            }

#include "filter/stencil.hxx"
            //! apply a matrix as a stencil
            template <typename T, typename U> static inline
            void Stencil(Pixmap<float>   &target,
                         const Pixmap<T> &source,
                         const matrix<U> &block,
                         Engine          &E)
            {
                __stencil<T,U> proxy = { &target, &source, &block };
                E.run(proxy);
            }


#include "filter/min-max.hxx"
            //! find minimum value
            template <typename T> static inline
            T FindMin( const Pixmap<T> &source, Engine &E )
            {
                E.acquire_all(sizeof(T));
                {
                    __find_min<T> proxy = { &source };
                    E.run(proxy);
                }
                return E.get_min<T>();
            }

            //! find maximum value
            template <typename T> static inline
            T FindMax( const Pixmap<T> &source, Engine &E )
            {
                E.acquire_all(sizeof(T));
                {
                    __find_max<T> proxy = { &source };
                    E.run(proxy);
                }
                return E.get_max<T>();
            }

            //! find min and max values
            template <typename T> static inline
            void FindMinMax( T &vmin, T&vmax, const Pixmap<T> &source, Engine &E )
            {
                E.acquire_all(2*sizeof(T));
                {
                    __find_min_max<T> proxy = { &source };
                    E.run(proxy);
                }
                return E.get_min_max(vmin,vmax);
            }

#include "filter/rescale.hxx"
            //! rescale using vmin and vmax, and a conversion: unit float to T
            template <typename T, typename F2T> static inline
            void Rescale( Pixmap<T> &source, const T vmin, const T vmax, F2T &float2type, Engine &E)
            {
                __rescale<T,F2T> proxy = { &source, vmin, vmax, &float2type };
                E.run(proxy);
            }

            //! rescale from global vmin and vmax
            template <typename T, typename F2T> static inline
            void Autoscale( Pixmap<T> &source, F2T &float2type, Engine &E)
            {
                float vmin=0,vmax=0;
                FindMinMax(vmin,vmax,source,E);
                Rescale(source,vmin,vmax,float2type,E);
            }

#include "filter/fill-holes.hxx"
            //! Fill Holes by interpolation
            template <typename T> static inline
            void FillHoles( Pixmap<T> &source, Engine &E )
            {
                __fill_holes<T> proxy = { &source };
                E.run(proxy);
            }

            //! Fill Holes by copy
            template <typename T> static inline
            void FillHoles( Pixmap<T> &target, const Pixmap<T> &source, Engine &E )
            {
                __fill_holes2<T> proxy = { &target, &source };
                E.run(proxy);
            }


#include "filter/average.hxx"
            //! replace pixel by the average of itself and its neighbors
            template <typename T> static inline
            void Average( Pixmap<T> &target, const Pixmap<T> &source, Engine &E)
            {
                __collect<T> proxy = { &target, &source, Pixel<T>::Average };
                E.run(proxy);
            }

            //! replace pixel by the median of itself and its neighbors
            template <typename T> static inline
            void Median( Pixmap<T> &target, const Pixmap<T> &source, Engine &E)
            {
                __collect<T> proxy = { &target, &source, Pixel<T>::Median };
                E.run(proxy);
            }

            //! replace pixel by the minimum value of itself and its neighbors
            template <typename T> static inline
            void Erode( Pixmap<T> &target, const Pixmap<T> &source, Engine &E)
            {
                __collect<T> proxy = { &target, &source, Pixel<T>::Erode };
                E.run(proxy);
            }

            //! replace pixel by the maximum value of itself and its neighbors
            template <typename T> static inline
            void Dilate( Pixmap<T> &target, const Pixmap<T> &source, Engine &E)
            {
                __collect<T> proxy = { &target, &source, Pixel<T>::Dilate };
                E.run(proxy);
            }
        };


    }
}


#endif

