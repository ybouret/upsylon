//! \file
#ifndef Y_MATH_FIT_LS_INCLUDED
#define Y_MATH_FIT_LS_INCLUDED 1

#include "y/math/fit/samples.hpp"
#include "y/sequence/arrays.hpp"

namespace upsylon
{
    namespace math
    {
        namespace Fit
        {

#define Y_LSF_OUT(CODE) if(verbose) do { CODE; } while(false)

            template <typename T>
            class LeastSquare : public arrays<T>
            {
            public:
                typedef typename Type<T>::Function Function;
                typedef typename Type<T>::Array    Array;
                typedef typename Type<T>::Matrix   Matrix;
                typedef typename Type<T>::Gradient Gradient;

                int      p;
                T        lam;
                Matrix   alpha;
                Matrix   curv;
                Array   &beta;
                Array   &delta;
                Array   &atry;
                Gradient grad;
                bool     verbose;

                inline explicit LeastSquare() :
                arrays<T>(10),
                p(0),
                lam(0),
                alpha(),
                beta(  this->next() ),
                delta( this->next() ),
                atry(  this->next() ),
                grad(),
                verbose(true)
                {
                }

                //! p < pmin => lam = 0
                static inline int get_pmin() throw()
                {
                    return -int(numeric<T>::dig);
                }

                //! p > pmax => overflow
                static inline int get_pmax() throw()
                {
                    return int(numeric<T>::max_10_exp);
                }



                inline bool compute_curvature()
                {
                    const size_t nvar = alpha.rows;
                    while(true)
                    {
                        curv.assign( alpha );
                        const T      fac = T(1) + lam;
                        for(size_t i=nvar;i>0;--i)
                        {
                            curv[i][i] *= fac;
                        }
                        if( !LU::build(curv) )
                        {
                            if(!increase_lambda())
                            {
                                return false;
                            }
                            else
                            {
                                continue; // with a new lambda
                            }
                        }
                        return true;
                    }
                }

                inline bool fit(Sample<T>         &sample,
                                Function          &F,
                                Array             &aorg,
                                Array             &aerr,
                                const array<bool> &used)
                {
                    assert(aerr.size()==aorg.size());
                    assert(used.size()==aorg.size());
                    const size_t nvar = aorg.size();
                    //__________________________________________________________
                    //
                    // init
                    //__________________________________________________________
                    if(verbose) { std::cerr << "[LSF] Initialize" << std::endl; }
                    aerr.ld(0);
                    if(nvar<=0)
                    {
                        Y_LSF_OUT(std::cerr << "[LSF] No Variables => SUCCESS" << std::endl);
                        return true;
                    }

                    this->acquire(nvar);
                    alpha.make(nvar,nvar);
                    curv.make(nvar,nvar);

                    p   = -4;
                    compute_lam();
                    Y_LSF_OUT(std::cerr << "[LSF] Initial lam=" << lam << "/p=" << p << std::endl);

                    alpha.ld(0);
                    beta.ld(0);
                    T D2_org = sample.computeD2(F,aorg,beta,alpha,grad,used);
                    while(true)
                    {
                        //______________________________________________________
                        //
                        // normalize alpha
                        //______________________________________________________
                        if(verbose) { std::cerr << "[LSF] D2_org=" << D2_org << "@" << aorg << std::endl; }
                        for(size_t i=nvar;i>0;--i)
                        {
                            if(used[i])
                            {
                                for(size_t j=i-1;j>0;--j)
                                {
                                    alpha[j][i] = alpha[i][j];
                                }
                            }
                            else
                            {
                                alpha[i][i] = 1; // for a null gradient
                            }
                        }
                        Y_LSF_OUT(std::cerr << "[LSF] descent and jacobian" << std::endl;
                                  std::cerr << "    beta  = "  << beta  << std::endl;
                                  std::cerr << "    alpha = " << alpha << std::endl);

                        //______________________________________________________
                        //
                        // compute curvature if possible
                        //______________________________________________________
                        if( !compute_curvature() )
                        {
                            Y_LSF_OUT(std::cerr << "[LSF] Singular Point" << std::endl);
                            return false;
                        }

                        //______________________________________________________
                        //
                        // compute step from this point
                        //______________________________________________________
                        tao::set(delta,beta);
                        LU::solve(curv,delta);
                        Y_LSF_OUT(std::cerr << "    delta = " << delta << std::endl);

                        //______________________________________________________
                        //
                        // probe new value
                        //______________________________________________________
                        tao::setprobe(atry, aorg,1,delta);
                        const T D2_try = sample.computeD2(F,atry);
                        Y_LSF_OUT(std::cerr << "[LSF] D2_try=" << D2_try << "@" << atry << std::endl);
                        if(D2_try>=D2_org)
                        {
                            std::cerr << "Increasing..." << std::endl;
                            exit(1);
                        }

                        // successfull step
                        decrease_lambda();
                        tao::set(aorg,atry);
                        alpha.ld(0);
                        beta.ld(0);
                        const T D2_err = D2_org - D2_try; assert(D2_err>=0);
                        D2_org = sample.computeD2(F,aorg,beta,alpha,grad,used);
                        if( D2_err <= numeric<T>::ftol * D2_org )
                        {
                            Y_LSF_OUT(std::cerr << "[LSF] D2 convergence" << std::endl);
                            goto CONVERGED;
                        }
                    }

                CONVERGED:
                    return false;
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(LeastSquare);

                //! initialize value of lam
                inline void compute_lam()
                {
                    if(p<0)
                    {
                        lam = ipower(T(0.1),size_t(-p));
                    }
                    else
                    {
                        if(p>0)
                        {
                            lam = ipower(T(10),size_t(p));
                        }
                        else
                        {
                            lam = 1;
                        }
                    }
                }

                bool increase_lambda() throw()
                {
                    static const int pmax = get_pmax();
                    if(++p>pmax) return false; //!< overflow
                    lam *= 10;
                    Y_LSF_OUT(std::cerr << "[LSF] (++) lam=" << lam << "/p=" << p << std::endl);
                    return true;
                }

                void decrease_lambda() throw()
                {
                    static const int pmin = get_pmin();

                    if(--p<pmin)
                    {
                        p   = pmin;
                        lam = 0;
                    }
                    else
                    {
                        lam *= T(0.1);
                    }
                    Y_LSF_OUT(std::cerr << "[LSF] (--) lam=" << lam << "/p=" << p << std::endl);

                }
            };

        }
    }

}

#endif

