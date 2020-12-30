//! \file

#ifndef Y_FITTING_SAMPLE_TYPE_INCLUDED
#define Y_FITTING_SAMPLE_TYPE_INCLUDED 1

#include "y/mkl/fitting/variables.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {

            //__________________________________________________________________
            //
            //
            //! sample type definition
            /**
             - ABSCISSA: abscissa type in 'R^d'
             - ORDINATE: ordinate type
             */
            //
            //__________________________________________________________________
            template <
            typename ABSCISSA,
            typename ORDINATE
            >
            class sample_type : public object, public counted
            {
            public:
                //______________________________________________________________
                //
                // virtual inteface
                //______________________________________________________________
                inline virtual       ~sample_type() throw() {}   //!< cleanup
                inline virtual size_t size()  const throw() = 0; //!< number of points

            protected:
                //! setup
                inline explicit sample_type() throw() : object(), counted()
                {
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(sample_type);
            };

        }

    }

}


#endif
