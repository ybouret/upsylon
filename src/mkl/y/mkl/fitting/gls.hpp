//! \file

#ifndef Y_MKL_FITTING_GLS_INCLUDED
#define Y_MKL_FITTING_GLS_INCLUDED 1

#include "y/mkl/fitting/samples.hpp"

namespace upsylon
{

    namespace mkl
    {
        namespace fitting
        {

            //__________________________________________________________________
            //
            //
            //! specialized, pre-compiled least squares
            //
            //__________________________________________________________________
            class gls
            {
            public:
                //______________________________________________________________
                //
                // types and definition
                //______________________________________________________________
                typedef sample_api<double,double> sample_api; //!< alias
                typedef sample<double,double>     sample;     //!< alias
                typedef sample::pointer           shared;     //!< alias
                typedef samples<double,double>    samples;    //!< alias
                typedef sequential<double,double> sequential; //!< alias
                typedef sequential::function      function;   //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit gls(const bool verb=false); //!< create
                virtual ~gls() throw();              //!< cleanup


            private:
                Y_DISABLE_COPY_AND_ASSIGN(gls);
                void *impl; //!< private implementation

            public:
                bool &verbose; //!< reference to least_squares verbosity

            };

        }
    }

}

#endif

