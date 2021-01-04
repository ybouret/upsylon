
//! \file

#ifndef Y_FITTING_LEAST_SQUARES_INCLUDED
#define Y_FITTING_LEAST_SQUARES_INCLUDED 1

#include "y/mkl/fitting/sample/api.hpp"
//#include "y/mkl/fitting/sequential/gradient.hpp"
#include "y/mkl/fitting/lambda.hpp"
#include "y/sequence/vector.hpp"
#include "y/mkl/kernel/lu.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {

            struct __least_squares
            {
                static const char prefix[];
            };

#define Y_GLS_PRINTLN(MSG) do { if(verbose) { std::cerr << __least_squares::prefix << MSG << std::endl; } } while(false)

            template <typename ABSCISSA, typename ORDINATE>
            class least_squares
            {
            public:
                typedef lambdas<ORDINATE>             lambdas_type;
                typedef matrix<ORDINATE>              matrix_type;
                typedef vector<ORDINATE>              vector_type;
                typedef sample_api<ABSCISSA,ORDINATE> sample_api_type;
                typedef sequential<ABSCISSA,ORDINATE> sequential_type;
                typedef v_gradient<ABSCISSA,ORDINATE> v_gradient_type;

                size_t             M;
                const lambdas_type lam;
                ORDINATE           lambda;
                int                p;
                matrix_type        alpha;
                matrix_type        curv;
                vector_type        beta;
                vector_type        aorg;
                vector_type        atry;
                vector_type        step;
                vector<bool>       used;
                bool               verbose;

                inline explicit least_squares() :
                M(0),
                lam(), lambda(0), p(0),
                alpha(),
                curv(),
                beta(),
                aorg(),
                atry(),
                step(),
                verbose(false)
                {
                }

                inline virtual ~least_squares() throw()
                {
                }

                inline bool fit(sample_api_type        &s,
                                sequential_type        &F,
                                v_gradient_type        &G,
                                addressable<ORDINATE>  &A,
                                const accessible<bool> &U)
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
                    curv.make(M,M);
                    beta.adjust(M,s.zero);
                    aorg.adjust(M,s.zero);
                    atry.adjust(M,s.zero);
                    step.adjust(M,s.zero);
                    used.adjust(M,false);
                    tao::set(aorg,A);
                    tao::set(used,U);
                    s.setup(A);

                    Y_GLS_PRINTLN("init: p=" << p << ", lambda=" << lambda);
                    if(verbose)
                    {
                        s.vars.display(std::cerr << "values:" << std::endl,aorg,"\t");
                        s.vars.display(std::cerr << "used  :" << std::endl,used,"\t");
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
                    Y_GLS_PRINTLN("D2_org = " << D2_org);
                    Y_GLS_PRINTLN("beta   = " << beta  );
                    Y_GLS_PRINTLN("alpha  = " << alpha  );

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



                    //----------------------------------------------------------
                    //
                    // compute trial D2
                    //
                    //----------------------------------------------------------
                    const double D2_try = s.D2(F,atry);
                    if(verbose)
                    {
                        s.vars.display(std::cerr << "\tstep:" << std::endl,step,"\t\t");
                        s.vars.display(std::cerr << "\tatry:" << std::endl,atry,"\t\t");
                    }
                    Y_GLS_PRINTLN("D2_try = " << D2_try);

                    if(D2_try>=D2_org)
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
                        goto CYCLE;
                    }


                CONVERGED:
                    // success
                    Y_GLS_PRINTLN("converged = " << converged);
                    Y_GLS_PRINTLN("lambda    = " << lambda);



                    return true;
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(least_squares);

                bool compute_step() throw()
                {
                TRY_COMPUTE:
                    const ORDINATE fac = ORDINATE(1) + lambda;
                    curv.assign(alpha);
                    for(size_t i=M;i>0;--i)
                    {
                        if(used[i])
                        {
                            curv[i][i] *= fac;
                        }
                    }
                    if(!LU::build(curv))
                    {
                        if(!increase())
                        {
                            Y_GLS_PRINTLN("singular curvature");
                            return false;
                        }
                        goto TRY_COMPUTE;
                    }
                    tao::set(step,beta);
                    LU::solve(curv,step);
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

