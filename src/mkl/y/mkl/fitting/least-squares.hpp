
//! \file

#ifndef Y_FITTING_LEAST_SQUARES_INCLUDED
#define Y_FITTING_LEAST_SQUARES_INCLUDED 1

#include "y/mkl/fitting/sample/api.hpp"
#include "y/mkl/fitting/lambda.hpp"
#include "y/mkl/fitting/sequential/gradient.hpp"
#include "y/mkl/fitting/variable/display.hpp"

#include "y/sequence/vector.hpp"
#include "y/mkl/kernel/lu.hpp"
#include "y/ptr/auto.hpp"
#include "y/code/textual.hpp"

#define Y_GLS_DEBUG 1
#if defined(Y_GLS_DEBUG) && 1 == Y_GLS_DEBUG
#include "y/ios/ocstream.hpp"
#endif

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
            struct __least_squares
            {
                static const char prefix[]; //!< "[fit] "
            };


            //! for verbose output
#define Y_GLS_PRINTLN(MSG) do { if(verbose) { std::cerr << __least_squares::prefix << MSG << std::endl; } } while(false)

            //__________________________________________________________________
            //
            //! least square algorithm
            //__________________________________________________________________
            template <typename ABSCISSA, typename ORDINATE>
            class least_squares
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef lambdas<ORDINATE>                      lambdas_type;     //!< alias
                typedef matrix<ORDINATE>                       matrix_type;      //!< alias
                typedef vector<ORDINATE>                       vector_type;      //!< alias
                typedef sample_api<ABSCISSA,ORDINATE>          sample_api_type;  //!< alias
                typedef sequential<ABSCISSA,ORDINATE>          sequential_type;  //!< alias
                typedef v_gradient<ABSCISSA,ORDINATE>          v_gradient_type;  //!< alias
                typedef sequential_gradient<ABSCISSA,ORDINATE> sequential_grad;  //!< alias
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
                beta(),
                aorg(),
                atry(),
                step(),
                used(),
                verbose(verbosity),
                grad_(0)
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
                                addressable<ORDINATE>  &E)
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
                // members
                //______________________________________________________________
                size_t                        M;       //!< number of parameters
                const lambdas_type            lam;     //!< array of precomputed lambdas
                ORDINATE                      lambda;  //!< current lambda
                int                           p;       //!< lambda=10^p
                matrix_type                   alpha;   //!< curvature
                matrix_type                   covar;   //!< covariance
                vector_type                   beta;    //!< beta
                vector_type                   aorg;    //!< starting point
                vector_type                   atry;    //!< trial point
                vector_type                   step;    //!< computed step
                vector_type                   atmp;    //!< for probing
                vector<bool>                  used;    //!< used parameters
                bool                          verbose; //!< output verbosity
                auto_ptr<sequential_grad>     grad_;   //!< internal gradient


            private:
                Y_DISABLE_COPY_AND_ASSIGN(least_squares);

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
                    // build the modified covariance matrix
                    const ORDINATE fac = ORDINATE(1) + lambda;
                    covar.assign(alpha);
                    for(size_t i=M;i>0;--i)
                    {
                        if(used[i])
                        {
                            covar[i][i] *= fac;
                        }
                    }

                    // try to invert it
                    if(!LU::build(covar))
                    {
                        decreasing = false;
                        if(!increase())
                        {
                            // well, ill conditionned problem...
                            Y_GLS_PRINTLN("<singular curvature>");
                            return false;
                        }
                        goto TRY_COMPUTE;
                    }

                    // compute step = inv(alpha)*beta
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

