
//! \file
#ifndef Y_MKL_OPTIMIZE_INCLUDED
#define Y_MKL_OPTIMIZE_INCLUDED 1

#include "y/mkl/opt/bracket.hpp"
#include "y/mkl/opt/minimize.hpp"


namespace upsylon {

    namespace mkl {

        //! automatic optimize
        template <typename T, typename FUNC> inline
        T optimize(FUNC &func, const T x0, const T x1, const T ftol = 0)
        {
            const T f0 = func(x0);
            const T f1 = func(x1);
            triplet<T> x = { x0, x1, x1 };
            triplet<T> f = { f0, f1, f1 };
            if( f1<f0 )
            {
                bracket::expand(func,x,f);
            }
            else
            {
                bracket::inside(func,x,f);
            }
            return minimize::run(func,x,f,ftol);
        }

    }
}
#endif

