
//! \file

#ifndef Y_FITTING_LEAST_SQUARES_INCLUDED
#define Y_FITTING_LEAST_SQUARES_INCLUDED 1

#include "y/mkl/fitting/sample/api.hpp"
#include "y/mkl/fitting/lambda.hpp"
#include "y/mkl/fitting/sequential/gradient.hpp"
#include "y/sequence/vector.hpp"
#include "y/mkl/kernel/lu.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {

            //------------------------------------------------------------------
            //
            //! common least squares data
            //
            //------------------------------------------------------------------
            struct __least_squares
            {
                static const char prefix[]; //!< "[fit] "
            };

            //! for verbose output
#define Y_GLS_PRINTLN(MSG) do { if(verbose) { std::cerr << __least_squares::prefix << MSG << std::endl; } } while(false)

            //------------------------------------------------------------------
            //
            //! least square algorithm
            //
            //------------------------------------------------------------------
            template <typename ABSCISSA, typename ORDINATE>
            class least_squares
            {
            public:
                typedef lambdas<ORDINATE>                      lambdas_type;     //!< alias
                typedef matrix<ORDINATE>                       matrix_type;      //!< alias
                typedef vector<ORDINATE>                       vector_type;      //!< alias
                typedef sample_api<ABSCISSA,ORDINATE>          sample_api_type;  //!< alias
                typedef sequential<ABSCISSA,ORDINATE>          sequential_type;  //!< alias
                typedef v_gradient<ABSCISSA,ORDINATE>          v_gradient_type;  //!< alias
                typedef sequential_gradient<ABSCISSA,ORDINATE> sequential_grad;  //!< alias
                typedef typename sequential_type::function     sequential_func;  //!< alias

                size_t             M;       //!< number of parameters
                const lambdas_type lam;     //!< array of precomputed lambdas
                ORDINATE           lambda;  //!< current lambda
                int                p;       //!< lambda=10^p
                matrix_type        alpha;   //!< curvature
                matrix_type        covar;   //!< covariance
                vector_type        beta;    //!< beta
                vector_type        aorg;    //!< starting point
                vector_type        atry;    //!< trial point
                vector_type        step;    //!< computed step
                vector<bool>       used;    //!< used parameters
                bool               verbose; //!< output verbosity


                //! setup
                inline explicit least_squares() :
                M(0),
                lam(), lambda(0), p(0),
                alpha(),
                covar(),
                beta(),
                aorg(),
                atry(),
                step(),
                used(),
                verbose(false),
                grad_(0)
                {
                }

                //! cleanup
                inline virtual ~least_squares() throw()
                {
                }

                //! internal gradient
                sequential_grad &grad()
                {
                    if(grad_.is_empty())
                    {
                        grad_ = new sequential_grad();
                    }
                    return *grad_;
                }


#include "least-squares.hxx"

                //! fit with internal gradient
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
                inline bool fit(sample_api_type        &s,
                                sequential_func        &f,
                                addressable<ORDINATE>  &A,
                                const accessible<bool> &U,
                                addressable<ORDINATE>  &E)
                {
                    sequential_function<ABSCISSA,ORDINATE> F(f);
                    return fit(s,F,A,U,E);
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(least_squares);
                auto_ptr<sequential_grad> grad_;

                //--------------------------------------------------------------
                //
                // compute the fitting step
                //
                //--------------------------------------------------------------
                bool compute_step() throw()
                {
                TRY_COMPUTE:
                    const ORDINATE fac = ORDINATE(1) + lambda;
                    covar.assign(alpha);
                    for(size_t i=M;i>0;--i)
                    {
                        if(used[i])
                        {
                            covar[i][i] *= fac;
                        }
                    }
                    if(!LU::build(covar))
                    {
                        if(!increase())
                        {
                            Y_GLS_PRINTLN("singular curvature");
                            return false;
                        }
                        goto TRY_COMPUTE;
                    }
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

