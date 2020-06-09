//! \file
#ifndef Y_MKL_ODE_TYPES_INCLUDED
#define Y_MKL_ODE_TYPES_INCLUDED 1

#include "y/functor.hpp"
#include "y/container/matrix.hpp"
#include "y/sequence/arrays.hpp"
#include "y/mkl/types.hpp"

namespace upsylon
{

    namespace math
    {
        namespace ODE
        {
            typedef memory::global memory_type; //!< memory for arrays

            //! proxy for types definition
            template <typename T> struct Field
            {
                typedef array<T>                                            Array;    //!< array of variables
                typedef matrix<T>                                           Matrix;   //!< alias for matrix
                typedef arrays<T,memory_type>                               Arrays;   //!< multiple arrays at once

                typedef functor<void,TL3(Array&,T,const Array&)>            Equation; //!< derivs( dydx[], x, y[] );
                typedef functor<void,TL4(Array&,Matrix&, T, const Array &)> Jacobian; //!< jacobn( dfdx[], dfdy[][], x, y[] );
                typedef functor<void,TL2(Array&,T)>                         Callback; //!< adjust trials y[]
                typedef functor<void,TL2(T,const Array&)>                   Collect;  //!< to use information (x,y[])

            };

        }

    }
}

#endif
