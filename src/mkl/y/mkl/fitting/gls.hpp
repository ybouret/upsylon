//! \file

#ifndef Y_MKL_FITTING_GLS_INCLUDED
#define Y_MKL_FITTING_GLS_INCLUDED 1

#include "y/mkl/fitting/samples.hpp"
#include "y/mkl/fitting/sequential/explode.hpp"
#include "y/mkl/fitting/least-squares.hpp"

namespace upsylon
{

    namespace mkl
    {
        namespace fitting
        {

            //! default general least square type
            typedef least_squares<double,double> gls_type;
            
            //__________________________________________________________________
            //
            //
            //! specialized, pre-compiled least squares
            //
            //__________________________________________________________________
            class gls : public gls_type
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
                // member
                //______________________________________________________________
                explode_solver_ptr solver; //!< available solver
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(gls);
                
            };

        }
    }

}

#endif

