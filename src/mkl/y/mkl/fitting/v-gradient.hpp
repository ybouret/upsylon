//! \file

#ifndef Y_FITTING_V_GRADIENT_INCLUDED
#define Y_FITTING_V_GRADIENT_INCLUDED 1

#include "y/mkl/fitting/variables.hpp"
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
            //! sequential calls interface
            //
            //__________________________________________________________________
            template <typename ABSCISSA, typename ORDINATE>
            class v_gradient
            {
            public:

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                //! cleanup
                inline virtual ~v_gradient() throw() {  ; }

            protected:
                //! setup
                inline explicit v_gradient() throw()  {}

            public:
                inline void operator()(addressable<ORDINATE>      &dFdA,
                                       const ABSCISSA              X,
                                       const accessible<ORDINATE> &aorg,
                                       const variables            &vars,
                                       const accessible<bool>     &used)
                {
                    tao::ld(dFdA,0);
                    compute(dFdA,X,aorg,vars,used);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(v_gradient);
                virtual void compute(addressable<ORDINATE>      &dFdA,
                                     const ABSCISSA              X,
                                     const accessible<ORDINATE> &aorg,
                                     const variables            &vars,
                                     const accessible<bool>     &used) = 0;
            };

        }

    }
}

#endif

