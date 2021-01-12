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
                typedef sample_api<double,double> sample_api_type; //!< alias
                typedef sample<double,double>     sample_type;     //!< alias
                typedef sample_type::pointer      shared;          //!< alias
                typedef samples<double,double>    samples_type;    //!< alias
                typedef sequential<double,double> sequential_type; //!< alias
                typedef sequential_type::function function;        //!< alias
                typedef v_gradient<double,double> v_gradient_type; //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit gls(const bool verb=false); //!< create
                virtual ~gls() throw();              //!< cleanup

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! fit sequential class and its v_gradient
                bool operator()(sample_api_type        &s,
                                sequential_type        &F,
                                v_gradient_type        &G,
                                addressable<double>    &A,
                                const accessible<bool> &U,
                                addressable<double>    &E);

                //! fit sequential class with internal gradient
                bool operator()(sample_api_type        &s,
                                sequential_type        &F,
                                addressable<double>    &A,
                                const accessible<bool> &U,
                                addressable<double>    &E);

                //! fit regular function with intenal gradient
                bool operator()(sample_api_type        &s,
                                function               &f,
                                addressable<double>    &A,
                                const accessible<bool> &U,
                                addressable<double>    &E);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(gls);
                void *impl; //!< private implementation

            public:
                bool   &verbose; //!< reference to least_squares verbosity
                double &scaling; //!< for internal gradient
            };

        }
    }

}

#endif

