#ifndef Y_MATH_ODE_TYPES_INCLUDED
#define Y_MATH_ODE_TYPES_INCLUDED 1

#include "y/functor.hpp"
#include "y/container/matrix.hpp"
#include "y/sequence/arrays.hpp"
#include "y/math/types.hpp"

namespace upsylon
{

    namespace math
    {
        namespace ODE
        {
            typedef memory::global memory_type;

            template <typename T> struct Field
            {
                typedef array<T>                                            Array;
                typedef matrix<T>                                           Matrix;
                typedef functor<void,TL3(Array&,T,const Array&)>            Equation; //!< derivs( dydx[], x, y[] );
                typedef functor<void,TL4(Array&,Matrix&, T, const Array &)> Jacobian; //!< jacobn( dfdx[], dfdy[][], x, y[] );
                typedef functor<void,TL2(Array&,T)>                         Callback; //!< adjust trials y[]
                typedef arrays<T,memory_type>                               Arrays;
            };

        }

    }
}

#endif
