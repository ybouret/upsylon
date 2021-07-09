
//! \file

#ifndef Y_FITTING_LEAST_SQUARES_INCLUDED
#define Y_FITTING_LEAST_SQUARES_INCLUDED 1

#include "y/mkl/fitting/sample/api.hpp"
#include "y/mkl/kernel/lambda.hpp"
#include "y/mkl/fitting/sequential/gradient.hpp"
#include "y/mkl/fitting/variable/display.hpp"
#include "y/sequence/arrays.hpp"
#include "y/mkl/kernel/lu.hpp"
#include "y/ptr/auto.hpp"
#include "y/code/textual.hpp"
#include "y/os/static-check.hpp"
#include "y/ios/ocstream.hpp"
#include <iomanip>

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {

            //__________________________________________________________________
            //
            //! common least squares data
            //__________________________________________________________________
            struct least_squares_
            {
                //______________________________________________________________
                //
                // contextual info
                //______________________________________________________________
                static const char prefix_spc[]; //!< "[FIT] "
                static const char prefix_tab[]; //!< "[FIT]\t"
                static const char writing_id[]; //!< "gls-log.dat"
            };


            //__________________________________________________________________
            //
            //! verbosity block
            //__________________________________________________________________
#define Y_GLS_VERBOSE(CODE) do { if(verbose) { CODE; } } while(false)

            //__________________________________________________________________
            //
            //! for verbose output
            //__________________________________________________________________
#define Y_GLS_PRINTLN(MSG) Y_GLS_VERBOSE(std::cerr << least_squares_::prefix_spc << MSG << std::endl)


            //__________________________________________________________________
            //
            //
            //! least square algorithm
            //
            //__________________________________________________________________
            template <typename ABSCISSA, typename ORDINATE>
            class least_squares
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef kernel::lambdas<ORDINATE>              lambdas_type;     //!< alias for precomputed lambda
                typedef matrix<ORDINATE>                       matrix_type;      //!< alias for numerical matrices
                typedef arrays<ORDINATE>                       arrays_type;      //!< alias
                typedef typename arrays_type::array_type       vector_type;      //!< alias
                typedef lightweight_array<bool>                flags_type;       //!< alias for booleans interface
                typedef sample_api<ABSCISSA,ORDINATE>          sample_api_type;  //!< alias for any sample
                typedef sequential<ABSCISSA,ORDINATE>          sequential_type;  //!< alias for the sequential interface
                typedef v_gradient<ABSCISSA,ORDINATE>          v_gradient_type;  //!< alias for the v_gradient interface
                typedef sequential_gradient<ABSCISSA,ORDINATE> sequential_grad;  //!< alias for the sequential gradient
                typedef typename sequential_type::function     sequential_func;  //!< alias
                
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                //! setup
                inline explicit least_squares(const bool verbosity=false) :
                M(0),
                lam(), lambda(0), p(0),
                alpha(),
                covar(),
                space(7),
                beta( space.next() ),
                aorg( space.next() ),
                atry( space.next() ),
                step( space.next() ),
                atmp( space.next() ),
                aerr( space.next() ),
                utmp( space.next() ),
                used(),
                grad_(0),
                verbose(verbosity),
                writing(false)
                {
                }

                //! cleanup
                inline virtual ~least_squares() throw()
                {
                }

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! internal "on-the-fly" numerical gradient
                /**
                 the scaling has to be checked
                 */
                sequential_grad &grad()
                {
                    if(grad_.is_empty())
                    {
                        grad_ = new sequential_grad();
                    }
                    return *grad_;
                }

                //! fractional error for variables
                static inline ORDINATE get_vtol() throw()
                {
                    return numeric<ORDINATE>::ftol;
                }
                
                //! fractional error for d2
                static inline ORDINATE get_dtol() throw()
                {
                    return numeric<ORDINATE>::sqrt_ftol;
                }
                
                //______________________________________________________________
                //
                // fit methods
                //______________________________________________________________
#include "least-squares.hxx"

                //! fit with internal gradient
                /**
                 the internal gradient is created or called
                 */
                inline bool fit(sample_api_type        &s,
                                sequential_type        &F,
                                addressable<ORDINATE>  &A,
                                const accessible<bool> &U,
                                addressable<ORDINATE>  &E )
                {
                    sequential_grad &G = grad();
                    G.F = &F;
                    return fit(s,F,G,A,U,E);
                }

                //! fit with a regular function
                /**
                 creates a wrapper and use the internal numerical gradient
                 */
                inline bool fit(sample_api_type        &s,
                                sequential_func        &f,
                                addressable<ORDINATE>  &A,
                                const accessible<bool> &U,
                                addressable<ORDINATE>  &E)
                {
                    sequential_function<ABSCISSA,ORDINATE> F(f);
                    return fit(s,F,A,U,E);
                }

                //______________________________________________________________
                //
                // solo errors methods
                //______________________________________________________________

                //! solo errors with internal gradient
                inline void errors(sample_api_type        &s,
                                   sequential_type        &F,
                                   addressable<ORDINATE>  &A,
                                   const accessible<bool> &U,
                                   addressable<ORDINATE>  &E)
                {
                    sequential_grad &G = grad();
                    G.F = &F;
                    errors(s,F,G,A,U,E);
                }


                //! fit with a regular function
                inline void errors(sample_api_type        &s,
                                   sequential_func        &f,
                                   addressable<ORDINATE>  &A,
                                   const accessible<bool> &U,
                                   addressable<ORDINATE>  &E)
                {
                    sequential_function<ABSCISSA,ORDINATE> F(f);
                    errors(s,F,A,U,E);
                }





            private:
                Y_DISABLE_COPY_AND_ASSIGN(least_squares);
                size_t                        M;       //!< max number of parameters
                const lambdas_type            lam;     //!< array of precomputed lambdas
                ORDINATE                      lambda;  //!< current lambda
                int                           p;       //!< lambda=10^p
                matrix_type                   alpha;   //!< curvature
                matrix_type                   covar;   //!< covariance
                arrays_type                   space;   //!< all vectors
                vector_type                  &beta;    //!< beta
                vector_type                  &aorg;    //!< starting point
                vector_type                  &atry;    //!< trial point
                vector_type                  &step;    //!< computed step
                vector_type                  &atmp;    //!< for probing
                vector_type                  &aerr;    //!< for errors
                vector_type                  &utmp;    //!< holds memory for used
                flags_type                    used;    //!< used parameters
                auto_ptr<sequential_grad>     grad_;   //!< internal gradient

            public:
                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                bool verbose; //!< output verbosity
                bool writing; //!< save lookup profiles

            private:
                //--------------------------------------------------------------
                //
                //
                // setup memory and values
                //
                //
                //--------------------------------------------------------------
                inline void setup_fields(sample_api_type        &s,
                                         addressable<ORDINATE>  &A,
                                         const accessible<bool> &U,
                                         addressable<ORDINATE>  &E)
                {
                    const variables &vars = s.vars; // variables for this run
                    M      = vars.sweep();          // dimensions

                    //----------------------------------------------------------
                    // memory
                    //----------------------------------------------------------
                    alpha.make(M,M);
                    covar.make(M,M);
                    space.acquire(M);
                    {
                        ORDINATE *tmp = *utmp;
                        new ( &used ) flags_type( (bool *)tmp, M );
                    }

                    //----------------------------------------------------------
                    // initialize values
                    //----------------------------------------------------------
                    tao::set(aorg,A);     // setup aorg
                    tao::ld(aerr,-s.one); // setup aerr
                    tao::ld(used,false);  // setup used
                    tao::set(used,U);     // fill used
                    vars.set(E,aerr);     // fill error values with -1
                    s.setup(aorg);        // and prepare sample(s)
                }

                //--------------------------------------------------------------
                //
                //
                // compute the fitting step
                //
                //
                //--------------------------------------------------------------
                bool compute_step(bool &decreasing) throw()
                {
                TRY_COMPUTE:
                    //----------------------------------------------------------
                    // build the modified covariance matrix
                    //----------------------------------------------------------
                    const ORDINATE fac = ORDINATE(1) + lambda;
                    covar.assign(alpha);
                    for(size_t i=M;i>0;--i)
                    {
                        if(used[i])
                        {
                            covar[i][i] *= fac;
                        }
                    }


                    //----------------------------------------------------------
                    // try to invert it
                    //----------------------------------------------------------
                    if(!LU::build(covar))
                    {
                        decreasing = false;
                        if(!increase())
                        {
                            //--------------------------------------------------
                            // well, ill conditionned problem...
                            //--------------------------------------------------
                            Y_GLS_PRINTLN("<singular curvature>");
                            return false;
                        }
                        goto TRY_COMPUTE;
                    }

                    //----------------------------------------------------------
                    // compute step = inv(alpha)*beta
                    //----------------------------------------------------------
                    tao::set(step,beta);
                    LU::solve(covar,step);
                    return true;
                }

                inline bool increase() throw()
                {
                    if(++p>lam.pmax) return false;
                    lambda = lam[p];
                    return true;
                }

                inline void decrease() throw()
                {
                    if(--p<=lam.pmin)
                    {
                        p=lam.pmin;
                    }
                    lambda = lam[p];
                }

            };

        }

    }

}


#endif

