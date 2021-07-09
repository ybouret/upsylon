//! \file

#ifndef Y_FITTING_SEQUENTIAL_FUNCTION_INCLUDED
#define Y_FITTING_SEQUENTIAL_FUNCTION_INCLUDED 1

#include "y/mkl/fitting/sequential.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {

            //__________________________________________________________________
            //
            //
            //! sequential calls interface of a regular function
            //
            //__________________________________________________________________
            template <typename ABSCISSA, typename ORDINATE>
            class sequential_function : public sequential<ABSCISSA,ORDINATE>
            {
            public:
                //! alias to a functor
                typedef typename sequential<ABSCISSA,ORDINATE>::function function;

                //! setup
                inline explicit sequential_function( function &host ) throw() : F(host) {}

                //! cleanup
                inline virtual ~sequential_function() throw() {}
                

            private:
                Y_DISABLE_COPY_AND_ASSIGN(sequential_function);
                function &F;

                virtual ORDINATE onStart(const ABSCISSA x, const accessible<ORDINATE> &aorg, const variables &vars)
                {
                    return F(x,aorg,vars);
                }

                virtual ORDINATE onReach(const ABSCISSA x, const accessible<ORDINATE> &aorg, const variables &vars)
                {
                    return F(x,aorg,vars);
                }
            };

        }

    }

}

#endif
