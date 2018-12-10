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

            //! macro to activate output on verbose flag
#define Y_LSF_OUT(CODE) if(verbose) do { CODE; } while(false)

            //! Least Squares fit
            template <typename T>
            class LeastSquares : public arrays<T>
            {
            public:
                typedef typename Type<T>::Function Function; //!< alias
                typedef typename Type<T>::Array    Array;    //!< alias
                typedef typename Type<T>::Matrix   Matrix;   //!< alias
                typedef typename Type<T>::Gradient Gradient; //!< alias

                int      p;       //!< controlled power for lam
                T        lam;     //!< lam=10^p
                Matrix   alpha;   //!< Jacobian of beta
                Matrix   curv;    //!< almost inverse of alpha
                Array   &beta;    //!< D2 descent direction
                Array   &delta;   //!< predicted step
                Array   &atry;    //!< trial position
                Gradient grad;    //!< gradient computation
                bool     verbose; //!< verbosity flag

                //! initialize LeastSquares
                inline explicit LeastSquares(const bool is_verbose=false) :
                arrays<T>(3),
                p(0),
                lam(0),
                alpha(),
                curv(),
                beta(  this->next() ),
                delta( this->next() ),
                atry(  this->next() ),
                grad(),
                verbose(is_verbose)
                {
                }

                //! destructor
                inline virtual ~LeastSquares() throw()
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

                //! try to fit sample
                inline bool fit(SampleType<T>     &sample,
                                Function          &F,
                                Array             &aorg,
                                Array             &aerr,
                                const array<bool> &used)
                {
                    assert(aerr.size()==aorg.size());
                    assert(used.size()==aorg.size());
                    const size_t nvar = aorg.size();
                    aerr.ld(0);

                    //__________________________________________________________
                    //
                    //
                    // initialize dynamic data and lambda
                    //
                    //__________________________________________________________
                    if(verbose) { std::cerr << "[LSF] initialize" << std::endl; }
                    if(nvar<=0)
                    {
                        Y_LSF_OUT(std::cerr << "[LSF] no variables" << std::endl);
                        return true;
                    }

                    this->acquire(nvar);
                    alpha.make(nvar,nvar);
                    curv. make(nvar,nvar);

                    p   = -4;
                    compute_lam();
                    Y_LSF_OUT(std::cerr << "[LSF] initial lambda=" << lam << "/p=" << p << std::endl);

                    alpha.ld(0);
                    beta.ld(0);
                    T D2 = sample.computeD2(F,aorg,beta,alpha,grad,used);
                    while(true)
                    {
                        //______________________________________________________
                        //
                        //
                        // start cycle: normalize alpha
                        //
                        //______________________________________________________
                        Y_LSF_OUT(std::cerr << "[LSF] D2=" << D2 << "@" << aorg << std::endl);
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
                        //
                        // compute curvature if possible, using current lam
                        //
                        //______________________________________________________
                    FIND_STEP:
                        if( !compute_curvature() )
                        {
                            Y_LSF_OUT(std::cerr << "[LSF] singular parameters" << std::endl);
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
                        tao::add(atry,aorg,delta);
                        const T D2_try = sample.computeD2(F,atry);
                        Y_LSF_OUT(std::cerr << "[LSF] D2_try=" << D2_try << "@" << atry << std::endl);

                        //______________________________________________________
                        //
                        // check if increasing D2
                        //______________________________________________________
                        if(D2_try>=D2)
                        {
                            Y_LSF_OUT(std::cerr << "[LSF] local minimum" << std::endl);
                            
                            //__________________________________________________
                            //
                            // test variable convergence
                            //__________________________________________________
                            bool converged = true;
                            for(size_t i=nvar;i>0;--i)
                            {
                                const T da = __fabs(aorg[i]-atry[i]);
                                if( da > numeric<T>::ftol * __fabs(aorg[i]))
                                {
                                    converged = false;
                                    break;
                                }
                            }
                            if(converged)
                            {
                                Y_LSF_OUT(std::cerr << "[LSF] variables convergence" << std::endl);
                                goto CONVERGED;
                            }
                            //__________________________________________________
                            //
                            // will reduce step by increasing lambda if possible
                            //__________________________________________________
                            if(!increase_lambda())
                            {
                                Y_LSF_OUT(std::cerr << "[LSF] spurious parameters" << std::endl);
                                goto CONVERGED; // local minimum...
                            }
                            goto FIND_STEP;
                        }

                        //______________________________________________________
                        //
                        // successfull step: update and test convergence
                        //______________________________________________________
                        tao::set(aorg,atry);
                        const T D2_err = __fabs(D2 - D2_try);
                        if( D2_err <= numeric<T>::sqrt_ftol * D2 )
                        {
                            Y_LSF_OUT(std::cerr << "[LSF] least squares convergence" << std::endl);
                            goto CONVERGED;
                        }

                        //______________________________________________________
                        //
                        // prepare next step: full computation
                        //______________________________________________________
                        decrease_lambda();
                        alpha.ld(0);
                        beta.ld(0);
                        D2 = sample.computeD2(F,aorg,beta,alpha,grad,used);
                    }

                CONVERGED:
                    //__________________________________________________________
                    //
                    // D2_org, aorg and alpha are computed
                    //__________________________________________________________
                    Y_LSF_OUT(std::cerr<<"[LSF] analyzing parameters"<<std::endl);
                    const size_t ndat = sample.count();

                    size_t nprm = nvar;
                    for(size_t j=nvar;j>0;--j)
                    {
                        if(!used[j]) --nprm;
                    }
                    Y_LSF_OUT(std::cerr << "|_#data       = " << ndat << std::endl);
                    Y_LSF_OUT(std::cerr << "|_#variables  = " << nvar << std::endl);
                    Y_LSF_OUT(std::cerr << "|_#parameters = " << nprm << std::endl);

                    if(nprm>ndat)
                    {
                        Y_LSF_OUT(std::cerr<< "[LSF] more parameters than data"<<std::endl);
                        aerr.ld(-1);
                        return false;
                    }
                    else if(nprm==ndat)
                    {
                        Y_LSF_OUT(std::cerr<< "[LSF] no degree of freedom: interpolation"<<std::endl);
                        aerr.ld(0);
                        return true;
                    }
                    assert(ndat>nprm);
                    const size_t ndof = ndat-nprm;
                    Y_LSF_OUT(std::cerr << "|_#dof        = " << ndof << std::endl);

                    Y_LSF_OUT(std::cerr<< "alpha=" << alpha << std::endl);
                    if(!LU::build(alpha))
                    {
                        Y_LSF_OUT(std::cerr << "[LSF] unexpected singular minimum" << std::endl);
                        return false;
                    }


                    LU::inverse(alpha,curv);
                    Y_LSF_OUT(std::cerr<< "curv=" << curv << std::endl);
                    const T dS = D2/ndof;
                    for(size_t i=nvar;i>0;--i)
                    {
                        if(used[i]) aerr[i] = sqrt_of( max_of<T>(0,dS*curv[i][i]) );
                    }
                    return true;
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(LeastSquares);

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
            };

        }
    }

}

#endif

