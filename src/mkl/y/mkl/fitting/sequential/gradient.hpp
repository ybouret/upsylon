//! \file

#ifndef Y_FITTING_SEQUENTIAL_GRADIENT_INCLUDED
#define Y_FITTING_SEQUENTIAL_GRADIENT_INCLUDED 1

#include "y/mkl/fitting/sequential.hpp"
#include "y/mkl/tao.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {

            //__________________________________________________________________
            //
            //
            //! sequential gradient interface
            //
            //__________________________________________________________________
            template <typename ABSCISSA, typename ORDINATE>
            class sequential_gradient
            {
            public:
                typedef sequential<ABSCISSA,ORDINATE> function_type;

            protected:
                inline explicit sequential_gradient() throw() {}

            public:
                inline virtual ~sequential_gradient() throw() {}

                inline void operator()(addressable<ORDINATE>      &dFdA,
                                       function_type              &F,
                                       const ABSCISSA              X,
                                       const accessible<ORDINATE> &aorg,
                                       const variables            &vars,
                                       const accessible<bool>     &used)
                {
                    tao::ld(dFdA,0);
                    return compute(dFdA,F,X,aorg,vars,used);
                }

                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(sequential_gradient);
                virtual void compute(addressable<ORDINATE>      &dFdA,
                                     function_type              &F,
                                     const ABSCISSA              X,
                                     const accessible<ORDINATE> &aorg,
                                     const variables            &vars,
                                     const accessible<bool>     &used) = 0;
            };

        }
    }
}

#endif
