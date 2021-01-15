//! \file

#ifndef Y_MKL_FITTING_GLS_INCLUDED
#define Y_MKL_FITTING_GLS_INCLUDED 1

#include "y/mkl/fitting/samples.hpp"
#include "y/mkl/fitting/sequential/explode.hpp"

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
                typedef sample_api<double,double>            sample_api_type; //!< alias
                typedef sample<double,double>                sample_type;     //!< alias
                typedef sample_type::pointer                 sample_ptr;      //!< alias
                typedef samples<double,double>               samples_type;    //!< alias

                typedef sequential<double,double>            sequential_type; //!< alias
                typedef sequential_type::function            function;        //!< alias
                typedef v_gradient<double,double>            v_gradient_type; //!< alias

                typedef explode<double>                      explode_type;        //!< alias
                typedef explode_type::solver_ptr             explode_solver_ptr;  //!< alias

                typedef ODE::ExplicitAdjust<double>          scheme_type;    //!< alias
                typedef scheme_type::Pointer                 scheme_ptr;     //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit gls(const bool verb=false); //!< create
                virtual ~gls() throw();              //!< cleanup

                //______________________________________________________________
                //
                // fit methods
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


                //______________________________________________________________
                //
                // solo errors methods
                //______________________________________________________________

                //! solo errors
                void solo(sample_api_type        &s,
                          sequential_type        &F,
                          v_gradient_type        &G,
                          addressable<double>    &A,
                          const accessible<bool> &U,
                          addressable<double>    &E);


                //! solo errors
                void solo(sample_api_type        &s,
                          sequential_type        &F,
                          addressable<double>    &A,
                          const accessible<bool> &U,
                          addressable<double>    &E);

                //! solo errors
                void solo(sample_api_type        &s,
                          function               &f,
                          addressable<double>    &A,
                          const accessible<bool> &U,
                          addressable<double>    &E);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(gls);
                void *impl; //!< private implementation

            public:
                bool                &verbose; //!< reference to least_squares verbosity
                double              &scaling; //!< internal gradient
                explode_solver_ptr  &solver;  //!< internal explicit driver
            };

        }
    }

}

#endif

