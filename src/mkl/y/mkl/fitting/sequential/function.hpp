
//! \file

#ifndef Y_FITTING_SEQUENTIAL_INCLUDED
#define Y_FITTING_SEQUENTIAL_INCLUDED 1

#include "y/mkl/fitting/sequential.hpp"
#include "y/functor.hpp"

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
            template <typename T,
            typename ABSCISSA,
            typename ORDINATE>
            class sequential_function : public sequential<T,ABSCISSA,ORDINATE>
            {
            public:
                typedef functor<ORDINATE,TL3(ABSCISSA,const accessible<T>&,const variables&) type;

                inline explicit sequential_function( type &host ) throw() : F(host) {}
                inline virtual ~sequential_function() throw() {}
                

            private:
                Y_DISABLE_COPY_AND_ASSIGN(sequential_function);
                type &F;

                virtual ORDINATE onStart(const ABSCISSA x, const accessible<T> &aorg, const variables &vars)
                {
                    return F(x,aorg,vars);
                }

                virtual ORDINATE onReach(const ABSCISSA x, const accessible<T> &aorg, const variables &vars)
                {
                    return F(x,aorg,vars);
                }
            };

        }

    }

}

#endif
