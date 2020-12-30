//! \file

#ifndef Y_FITTING_SAMPLES_INCLUDED
#define Y_FITTING_SAMPLES_INCLUDED 1

#include "y/mkl/fitting/sample.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {

            //__________________________________________________________________
            //
            //
            //! sample for a set of data
            //
            //__________________________________________________________________
            template <
            typename ABSCISSA,
            typename ORDINATE
            >
            class samples : public sample_type<ABSCISSA,ORDINATE>,
            public hash_set<string, typename sample<ABSCISSA,ORDINATE>::pointer>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef sample_type<ABSCISSA,ORDINATE>    self_type;
                typedef sample<ABSCISSA,ORDINATE>         single_sample;
                typedef typename single_sample::pointer   shared_sample;
                typedef hash_set<string,shared_sample>    samples_db;

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                template <typename ID>
                inline explicit samples(const ID &id) : self_type(id), samples_db() {}
                inline virtual ~samples() throw() {}

                //______________________________________________________________
                //
                // sample_interface
                //______________________________________________________________
                inline virtual size_t count() const throw() { return 0; }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(samples);
            };

        }

    }

}
#endif

