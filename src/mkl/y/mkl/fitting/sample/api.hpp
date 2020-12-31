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
             - ABSCISSA: abscissa type in 'REAL^d'
             - ORDINATE: ordinate type, float or double
             */
            //
            //__________________________________________________________________
            template <
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
                typedef sequential<ABSCISSA,ORDINATE> sequential_type; //!< alias

                //______________________________________________________________
                //
                // virtual inteface
                //______________________________________________________________
                inline virtual  ~sample_api()   throw() {}   //!< cleanup
                virtual size_t   count()  const throw() = 0; //!< number of points
                virtual void     setup()                = 0; //!< prepare for a cycle
                virtual ORDINATE D2(sequential_type &, const accessible<ORDINATE> &) = 0; //!< compute D2 and all adjusted var


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
                inline explicit sample_api( const ID &id):
                object(),
                counted(),
                name(id),
                vars()
                {
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(sample_api);
            };

        }

    }

}


#endif
