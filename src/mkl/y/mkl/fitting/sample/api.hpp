//! \file

#ifndef Y_FITTING_SAMPLE_API_INCLUDED
#define Y_FITTING_SAMPLE_API_INCLUDED 1

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
            //! sample type definition
            /**
             - T       : type for parameters
             - ABSCISSA: abscissa type in 'R^d'
             - ORDINATE: ordinate type
             */
            //
            //__________________________________________________________________
            template <
            typename T,
            typename ABSCISSA,
            typename ORDINATE
            >
            class sample_api : public object, public counted
            {
            public:
                //______________________________________________________________
                //
                // types and definition
                //______________________________________________________________
                
                //______________________________________________________________
                //
                // virtual inteface
                //______________________________________________________________
                inline virtual       ~sample_api()   throw() {}   //!< cleanup
                inline virtual size_t count()  const throw() = 0; //!< number of points

                //______________________________________________________________
                //
                // non-virtual inteface
                //______________________________________________________________
                //! key for sets and intr_ptr
                inline const string &key() const throw() { return name; }

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const string name; //!< unique identifier
                variables    vars; //!< variables to pass to objective function

            protected:
                //! setup
                template <typename ID>
                inline explicit sample_api( const ID &id) throw() : object(), counted(), name(id)
                {
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(sample_api);
            };

        }

    }

}


#endif
