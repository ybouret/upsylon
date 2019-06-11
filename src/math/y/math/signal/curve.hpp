//! \file

#ifndef Y_SIGNAL_CURVE_INCLUDED
#define Y_SIGNAL_CURVE_INCLUDED 1

#include "y/type/point3d.hpp"

namespace upsylon
{
    namespace math
    {
        struct curve
        {
            template <typename T> struct info_for;

            template <> struct info_for<float>
            {
                static const size_t dim = 1;
                typedef float       real;
            };

            template <> struct info_for< point2d<float> >
            {
                static const size_t dim = 2;
                typedef float       real;
            };

            template <> struct info_for< point3d<float> >
            {
                static const size_t dim = 3;
                typedef float       real;
            };

            template <> struct info_for<double>
            {
                static const size_t dim = 1;
                typedef double      real;
            };

            template <> struct info_for< point2d<double> >
            {
                static const size_t  dim = 2;
                typedef double       real;
            };

            template <> struct info_for< point3d<double> >
            {
                static const size_t dim = 3;
                typedef double      real;
            };



        };
    }
}


#endif

