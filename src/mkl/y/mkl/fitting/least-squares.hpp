
//! \file

#ifndef Y_FITTING_LEAST_SQUARES_INCLUDED
#define Y_FITTING_LEAST_SQUARES_INCLUDED 1

#include "y/mkl/fitting/sample/api.hpp"
#include "y/mkl/fitting/lambda.hpp"
#include "y/sequence/vector.hpp"
#include "y/mkl/kernel/lu.hpp"

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
                typedef lambdas<ORDINATE>             lambdas_type;     //!< alias
                typedef matrix<ORDINATE>              matrix_type;      //!< alias
                typedef vector<ORDINATE>              vector_type;      //!< alias
                typedef sample_api<ABSCISSA,ORDINATE> sample_api_type;  //!< alias
                typedef sequential<ABSCISSA,ORDINATE> sequential_type;  //!< alias
                typedef v_gradient<ABSCISSA,ORDINATE> v_gradient_type;  //!< alias

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
                verbose(false)
                {
                }

                //! cleanup
                inline virtual ~least_squares() throw()
                {
                }

                //______________________________________________________________
                //
                //! generic call
                /**
                 \param s a sample interface
                 \param F a sequential function
                 \param G a gradient of F
                 \param A array of parameters
                 \param U array of flags to use parameters
                 \param E array of errors
                 */
                //______________________________________________________________
                inline bool fit(sample_api_type        &s,
                                sequential_type        &F,
                                v_gradient_type        &G,
                                addressable<ORDINATE>  &A,
                                const accessible<bool> &U,
                                addressable<ORDINATE>  &E)
                {
                    //----------------------------------------------------------
                    //
                    //
                    // initialize
                    //
                    //
                    //----------------------------------------------------------
                    assert(A.size()==U.size());
                    M      = A.size();
                    p      = 0;
                    lambda = lam[p];
                    alpha.make(M,M);
                    covar.make(M,M);
                    beta.adjust(M,s.zero);
                    aorg.adjust(M,s.zero);
                    atry.adjust(M,s.zero);
                    step.adjust(M,s.zero);
                    used.adjust(M,false);
                    tao::set(aorg,A);
                    tao::set(used,U);
                    tao::ld(E,-1);
                    s.setup(A);

                    Y_GLS_PRINTLN("init: p=" << p << ", lambda=" << lambda);
                    if(verbose)
                    {
                        s.vars.display(std::cerr,aorg,used," : ","\t(*) ","");
                    }

                    //----------------------------------------------------------
                    //
                    //
                    // cycle
                    //
                    //
                    //----------------------------------------------------------
                    size_t cycle=0;
                CYCLE:
                    ++cycle;
                    ORDINATE D2_org = s.D2(alpha,beta,F,G,aorg,U);
                    Y_GLS_PRINTLN("<cycle> = " << cycle  );
                    Y_GLS_PRINTLN("D2_org  = " << D2_org );
                    Y_GLS_PRINTLN("beta    = " << beta   );
                    Y_GLS_PRINTLN("alpha   = " << alpha  );

                    //----------------------------------------------------------
                    //
                    // compute curvature with lamba increase
                    //
                    //----------------------------------------------------------
                COMPUTE_STEP:
                    if(!compute_step())
                    {
                        return false;
                    }

                    //----------------------------------------------------------
                    //
                    // compute atry
                    //
                    //----------------------------------------------------------
                    for(size_t i=M;i>0;--i)
                    {
                        atry[i] = aorg[i] + step[i];
                    }

                    //----------------------------------------------------------
                    //
                    // TODO: control atry
                    //
                    //----------------------------------------------------------

                    //----------------------------------------------------------
                    //
                    // recompute step and check cvg
                    //
                    //----------------------------------------------------------
                    bool converged = true;
                    for(size_t i=M;i>0;--i)
                    {
                        const ORDINATE a_new = atry[i];
                        const ORDINATE a_old = aorg[i];
                        const ORDINATE da    = fabs_of( step[i] = a_new - a_old );
                        if( da > numeric<double>::ftol * max_of( fabs_of(a_new), fabs_of(a_old) ) )
                        {
                            converged = false;
                        }
                    }
                    Y_GLS_PRINTLN("converged/variable = " << converged);



                    //----------------------------------------------------------
                    //
                    // compute trial D2
                    //
                    //----------------------------------------------------------
                    const ORDINATE D2_try = s.D2(F,atry);
                    if(verbose)
                    {
                        s.vars.display(std::cerr,atry,step," (","\t(*) ",")");
                    }
                    Y_GLS_PRINTLN("D2_try = " << D2_try << " @ <lambda=" << lambda << "> <cycle=" << cycle << ">");

                    if(D2_try>D2_org)
                    {
                        //------------------------------------------------------
                        // reject
                        //------------------------------------------------------
                        if(!increase())
                        {
                            return false;
                        }
                        goto COMPUTE_STEP;
                    }
                    else
                    {
                        //------------------------------------------------------
                        // accept
                        //------------------------------------------------------
                        tao::set(aorg,atry);
                        decrease();

                        if(converged)
                        {
                            goto CONVERGED;
                        }

                        const ORDINATE dd = fabs_of(D2_org-D2_try);
                        if( dd <= numeric<ORDINATE>::sqrt_ftol * max_of(D2_org,D2_try) )
                        {
                            Y_GLS_PRINTLN("<D2 convergence>");
                            goto CONVERGED;
                        }

                        goto CYCLE;
                    }


                CONVERGED:
                    //----------------------------------------------------------
                    //
                    //
                    // success ?
                    //
                    //
                    //----------------------------------------------------------
                    Y_GLS_PRINTLN("lambda    = " << lambda);

                    //----------------------------------------------------------
                    //
                    // final D2
                    //
                    //----------------------------------------------------------
                    D2_org = s.D2(alpha,beta,F,G,aorg,used);

                    //----------------------------------------------------------
                    //
                    // compute covariance
                    //
                    //----------------------------------------------------------
                    if(!LU::build(alpha))
                    {
                        Y_GLS_PRINTLN("singular extremum");
                        return false;
                    }


                    LU::inverse(alpha,covar);
                    Y_GLS_PRINTLN("covar    = " << covar);

                    //----------------------------------------------------------
                    //
                    // set new parameters
                    //
                    //----------------------------------------------------------
                    tao::set(A,aorg);


                    //----------------------------------------------------------
                    //
                    // compute d.o.f
                    //
                    //----------------------------------------------------------
                    size_t ndof = s.count();
                    size_t nuse = 0;
                    for(size_t i=M;i>0;--i)
                    {
                        if(used[i]) ++nuse;
                    }

                    if(nuse>ndof)
                    {
                        //------------------------------------------------------
                        //
                        // meaningless, leave error to -1
                        //
                        //------------------------------------------------------
                        Y_GLS_PRINTLN("<meaningless>" );
                        return true;
                    }
                    else if(nuse==ndof)
                    {
                        //------------------------------------------------------
                        //
                        // interpolation, set error to 0
                        //
                        //------------------------------------------------------
                        Y_GLS_PRINTLN("<interpolation>");
                        tao::ld(E,s.zero);
                        return true;
                    }
                    else
                    {
                        //------------------------------------------------------
                        //
                        // successfull, compute individual errors
                        //
                        //------------------------------------------------------
                        assert(ndof>nuse);
                        Y_GLS_PRINTLN("<success>");
                        ndof -= nuse;
                        const size_t n2 = ndof*ndof;
                        for(size_t i=M;i>0;--i)
                        {
                            if(used[i])
                            {
                                E[i] = sqrt_of( D2_org * max_of<ORDINATE>(0,covar[i][i]) / n2 );
                            }
                            else
                            {
                                E[i] = s.zero;
                            }
                        }
                        if(verbose)
                        {
                            s.vars.display(std::cerr,A,E," \\pm ","\t(*) ","");
                        }
                        return true;
                    }


                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(least_squares);

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

