//! \file
#ifndef Y_MATH_FIT_LS_INCLUDED
#define Y_MATH_FIT_LS_INCLUDED 1

#include "y/math/fit/samples.hpp"
#include "y/sequence/arrays.hpp"
#include "y/math/kernel/lu.hpp"
#include "y/math/kernel/tao.hpp"

namespace upsylon
{
    namespace math
    {
        namespace Fit
        {

            ////////////////////////////////////////////////////////////////////
            //
            //! macro to activate output on verbose flag
#define Y_LSF_OUT(CODE)     if(verbose) do { CODE; } while(false)
            //
            //! Least Squares fit
            //
            ////////////////////////////////////////////////////////////////////
            template <typename T>
            class LeastSquares : public arrays<T>
            {
            public:
                //______________________________________________________________
                //
                //
                // types and definition
                //
                //______________________________________________________________
                typedef typename Type<T>::Function   Function;   //!< alias
                typedef typename Type<T>::Sequential Sequential; //!< alias
                typedef typename Type<T>::Array      Array;      //!< alias
                typedef typename Type<T>::Matrix     Matrix;     //!< alias
                typedef typename Type<T>::Gradient   Gradient;   //!< alias

                //______________________________________________________________
                //
                //
                // members
                //
                //______________________________________________________________
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
                inline virtual ~LeastSquares() throw() {}

                //! p < pmin => lam = 0
                static inline int get_pmin() throw() { return -int(numeric<T>::dig); }

                //! p > pmax => rise overflow exception in code
                static inline int get_pmax() throw() { return int(numeric<T>::max_10_exp); }

                //! initial p value
                static inline int get_pini() throw() { return -4; }

                //! try to fit sample
                inline bool fit(SampleType<T>     &sample,
                                Sequential        &F,
                                Array             &aorg,
                                Array             &aerr,
                                const array<bool> &used)
                {
                    static const T  ftol = T(1e-4);

                    assert(aerr.size()==aorg.size());
                    assert(used.size()==aorg.size());
                    const size_t nvar = aorg.size();



                    //__________________________________________________________
                    //
                    //
                    // initialize dynamic data and lambda
                    //
                    //__________________________________________________________
                    Y_LSF_OUT(std::cerr << "[LSF] \tInitializing for #variables=" << nvar << std::endl);
                    aerr.ld(0);        // no error
                    sample.prepare();  // internal memory and indexing

                    //----------------------------------------------------------
                    // check space size
                    //----------------------------------------------------------
                    if(nvar<=0)
                    {
                        Y_LSF_OUT(std::cerr << "[LSF] \t[- No Variables -]" << std::endl);
                        return true;
                    }

                    //----------------------------------------------------------
                    // check space size
                    //----------------------------------------------------------
                    this->acquire(nvar);   // vectors
                    alpha.make(nvar,nvar); // hessian
                    curv. make(nvar,nvar); // curvature

                    //----------------------------------------------------------
                    // initial step damping
                    //----------------------------------------------------------
                    p   = get_pini();
                    compute_lam();
                    Y_LSF_OUT(std::cerr << "[LSF] \tStarting with lambda=" << lam << "/p=" << p << std::endl);

                    //----------------------------------------------------------
                    // full initial metrics
                    //----------------------------------------------------------
                    alpha.ld(0);
                    beta.ld(0);
                    Y_LSF_OUT(std::cerr << "[LSF] \tComputing initial gradient..." << std::endl);
                    T D2 = sample.computeD2(F,aorg,beta,alpha,grad,used);
                    unsigned cycle = 0;
                    Y_LSF_OUT(std::cerr << "[LSF] \tReady for first cycle!" << std::endl);

                    while(true)
                    {
                        ++cycle;
                        //______________________________________________________
                        //
                        //
                        // start cycle: normalize alpha
                        //
                        //______________________________________________________
                        Y_LSF_OUT(std::cerr << "|" << std::endl;
                                  std::cerr << "[LSF] \t<cycle #" << cycle << ">" << std::endl;
                                  std::cerr << "[LSF] \tD2=" << D2 << " @" << aorg << std::endl);
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
                                beta[i]     = 0; // mandatory
                            }
                        }
                        Y_LSF_OUT(std::cerr << "[LSF] \tDescent, Hessian and Step:" << std::endl;
                                  std::cerr << "      \tbeta  = "  << beta  << std::endl;
                                  std::cerr << "      \talpha = " << alpha << std::endl);

                        //______________________________________________________
                        //
                        //
                        // compute curvature if possible, using current lam
                        //
                        //______________________________________________________
                    FIND_STEP:
                        if( !compute_curvature() )
                        {
                            Y_LSF_OUT(std::cerr << "[LSF] \tSingular Parameters" << std::endl);
                            return false;
                        }

                        //______________________________________________________
                        //
                        // compute step from this point
                        //______________________________________________________
                        tao::set(delta,beta);
                        LU::solve(curv,delta);
                        Y_LSF_OUT(std::cerr << "      \tdelta = " << delta << std::endl);

                        //______________________________________________________
                        //
                        // probe new value
                        //______________________________________________________
                        tao::add(atry,aorg,delta);
                        const T D2_try = sample.computeD2(F,atry);
                        Y_LSF_OUT(std::cerr << "[LSF] \tD2_try=" << D2_try << "@" << atry << " / D2=" << D2 << std::endl);

                        //______________________________________________________
                        //
                        // check if increasing D2
                        //______________________________________________________
                        if(D2_try>=D2)
                        {
                            Y_LSF_OUT(std::cerr << "[LSF] \tLocal Extremum:" << std::endl);
                            
                            //__________________________________________________
                            //
                            // test variable convergence on INCREASING D2
                            //__________________________________________________
                            const T ferr = tao::fractional_error(aorg,atry);
                            Y_LSF_OUT(std::cerr << "[LSF] \tError on Parameters: [ " << ferr << " ]" << std::endl);

                            if(ferr<=numeric<T>::sqrt_ftol)
                            {
                                Y_LSF_OUT(std::cerr << "[LSF] \t[- Convergence of Parameters -]" << std::endl);
                                goto CONVERGED;
                            }
                            //__________________________________________________
                            //
                            // will reduce step by increasing lambda if possible
                            //__________________________________________________
                            if(!increase_lambda())
                            {
                                Y_LSF_OUT(std::cerr << "[LSF] \t[- Spurious Parameters -]" << std::endl);
                                goto CONVERGED; // local minimum...
                            }
                            goto FIND_STEP;
                        }

                        //______________________________________________________
                        //
                        // successfull step: update and test convergence
                        //______________________________________________________
                        tao::set(aorg,atry);
                        const T D2_err = fabs_of(D2 - D2_try);
                        Y_LSF_OUT(std::cerr << "[LSF] \tLeastSquares Error : [ " << D2_err/D2 << " ]" << std::endl);

                        if( D2_err <= ftol * D2 )
                        {
                            Y_LSF_OUT(std::cerr << "[LSF] \t[+ Least Squares Convergence +]" << std::endl);
                            goto CONVERGED;
                        }


                        //______________________________________________________
                        //
                        // prepare next step: full computation
                        //______________________________________________________
                        decrease_lambda();
                        alpha.ld(0);
                        beta.ld(0);
                        Y_LSF_OUT(std::cerr << "[LSF] \tUpdating gradient..." << std::endl);
                        D2 = sample.computeD2(F,aorg,beta,alpha,grad,used); // will be D2_try
                        Y_LSF_OUT(std::cerr << "[LSF] \t<cycle #" << cycle << "/>" << std::endl);
                    }

                CONVERGED:
                    //__________________________________________________________
                    //
                    // D2_org, aorg and alpha are computed
                    //__________________________________________________________
                    Y_LSF_OUT(std::cerr<<"[LSF] \tAnalysis after #cycles="<< cycle << std::endl);
                    const size_t ndat = sample.count();

                    size_t nprm = nvar;
                    for(size_t j=nvar;j>0;--j)
                    {
                        if(!used[j]) --nprm;
                    }
                    Y_LSF_OUT(std::cerr << "    |_#data       = " << ndat << std::endl);
                    Y_LSF_OUT(std::cerr << "    |_#variables  = " << nvar << std::endl);
                    Y_LSF_OUT(std::cerr << "    |_#parameters = " << nprm << std::endl);

                    if(nprm>ndat)
                    {
                        Y_LSF_OUT(std::cerr<< "[LSF] \tMore parameters than data"<<std::endl);
                        aerr.ld(-1);
                        return false;
                    }
                    else if(nprm==ndat)
                    {
                        Y_LSF_OUT(std::cerr<< "[LSF] \tNo degree of freedom: interpolation"<<std::endl);
                        aerr.ld(0);
                        return true;
                    }
                    assert(ndat>nprm);
                    const size_t ndof = ndat-nprm;
                    Y_LSF_OUT(std::cerr << "    |_#dof        = " << ndof << std::endl);
                    Y_LSF_OUT(std::cerr << "    |_alpha=" << alpha << std::endl);
                    if(!LU::build(alpha))
                    {
                        Y_LSF_OUT(std::cerr << "[LSF] unexpected singular minimum" << std::endl);
                        return false;
                    }


                    LU::inverse(alpha,curv);
                    Y_LSF_OUT(std::cerr<< "    |_curv=" << curv << std::endl);
                    const T dS = D2/ndof;
                    for(size_t i=nvar;i>0;--i)
                    {
                        if(used[i]) aerr[i] = sqrt_of( max_of<T>(0,dS*curv[i][i]) );
                    }
                    return true;
                }

                //! wrapper to use regular function
                inline bool fit(SampleType<T>     &sample,
                                Function          &F,
                                Array             &aorg,
                                Array             &aerr,
                                const array<bool> &used)
                {
                    typename Type<T>::SequentialFunction SF(F);
                    return fit(sample,SF,aorg,aerr,used);
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
                    compute_lam();
                    Y_LSF_OUT(std::cerr << "[LSF] \t(+) lam=" << lam << "/p=" << p << std::endl);
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
                        compute_lam();
                    }
                    Y_LSF_OUT(std::cerr << "[LSF] \t(-) lam=" << lam << "/p=" << p << std::endl);
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

