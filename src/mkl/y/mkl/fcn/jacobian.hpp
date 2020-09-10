//! \file

#ifndef Y_MATH_FCN_JACOBIAN_INCLUDED
#define Y_MATH_FCN_JACOBIAN_INCLUDED 1

#include "y/container/matrix.hpp"
#include "y/functor.hpp"

namespace upsylon
{
    namespace mkl
    {
        //______________________________________________________________________
        //
        //
        //! jacobian interface
        //
        //______________________________________________________________________
        template <typename T>
        struct jacobian
        {
            typedef functor<void,TL2(matrix<T>&,const accessible<T> &)> type;
        };
       
    }
}


#endif

