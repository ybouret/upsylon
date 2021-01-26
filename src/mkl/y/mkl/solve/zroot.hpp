
//! \file

#ifndef Y_MKL_SOLVE_ZROOT_INCLUDED
#define Y_MKL_SOLVE_ZROOT_INCLUDED 1

#include "y/mkl/solve/zdriver.hpp"
#include "y/mkl/solve/zbis.hpp"
#include "y/mkl/solve/zsec.hpp"
#include "y/mkl/solve/zrid.hpp"

namespace upsylon
{
    namespace mkl
    {

        template <typename T>
        struct zroot
        {
            typedef zdriver<T,zbis> bisection;
            typedef zdriver<T,zsec> secant;
            typedef zdriver<T,zrid> ridder;
        };

    }

}

#endif
