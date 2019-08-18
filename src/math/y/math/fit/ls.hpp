//! \file
#ifndef Y_MATH_FIT_LS_INCLUDED
#define Y_MATH_FIT_LS_INCLUDED 1

#include "y/math/fit/samples.hpp"
#include "y/sequence/arrays.hpp"
#include "y/math/kernel/lu.hpp"
#include "y/math/kernel/tao.hpp"

#include "y/math/opt/minimize.hpp"
#include "y/math/opt/bracket.hpp"

namespace upsylon
{
    namespace math
    {
        namespace Fit
        {

            //! fractional tolerance on least-square
            /**
             not necessary to be too demanding for a decrease around the
             minimum is not statistically significant.
             */
#define Y_LSF_FTOL 1e-5

            //! fractional tolerance for damping
            /**
             I test if D2 is decreasing at a fraction (1-Y_LSF_UTOL) of the full
             step. If so, I backtrack to optimize the cost of the gradient computation
             and to avoid wandering.
             */
#define Y_LSF_UTOL 1e-3

            //! line size in case of verbosity
#define Y_LSF_LINE 72

            //! base class for utilities
            class LeastSquares_
            {
            public:
                static const int    InitialExponent = -4; //!< initial bias
                static const size_t InternalArrays  =  4; //!< sub arrays: beta/delta/atry/delta_cut
                virtual ~LeastSquares_() throw();          //!< destructor

                //! shared text
                static const char *ConvergedText(const bool flag) throw();

                //! yes or no
                static const char *YesOrNoText(const bool flag) throw();

                //! draw separator line
                static std::ostream &OutputLine( std::ostream &, size_t n );


            protected:
                explicit LeastSquares_() throw(); //!< constructor

            private:
                Y_DISABLE_COPY_AND_ASSIGN(LeastSquares_);
            };

            ////////////////////////////////////////////////////////////////////
            //
            //! macro to activate output on verbose flag
#define Y_LSF_OUT(CODE)     if(verbose) do { CODE; } while(false)
            //
            //! Least Squares fit
            //
            ////////////////////////////////////////////////////////////////////
            template <typename T>
            class LeastSquares : public LeastSquares_, public arrays<T>
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
                typedef typename Type<T>::Validate   Validate;   //!< alias

                //______________________________________________________________
                //
                //
                // members
                //
                //______________________________________________________________
                int      p;         //!< controlled power for lam
                T        lam;       //!< lam=10^p
                Matrix   alpha;     //!< Jacobian of beta
                Matrix   curv;      //!< almost inverse of alpha
                Array   &beta;      //!< D2 descent direction
                Array   &delta;     //!< predicted step
                Array   &atry;      //!< trial position
                Array   &bound;     //!< in case of control
                Gradient grad;      //!< gradient computation
                bool     verbose;   //!< verbosity flag


                //! initialize LeastSquares
                inline explicit LeastSquares(const bool is_verbose=false) :
                arrays<T>(InternalArrays),
                p(0),
                lam(0),
                alpha(),
                curv(),
                beta(  this->next() ),
                delta( this->next() ),
                atry(  this->next() ),
                bound( this->next() ),
                grad(),
                verbose(is_verbose)
                {
                }

                //! destructor
                inline virtual ~LeastSquares() throw() {}

                //! (p < pmin) => (lam = 0)
                static inline int get_min_exponent() throw() { return -int(numeric<T>::dig);       }

                //! (p > pmax) => overflow and early return
                static inline int get_max_exponent() throw() { return int(numeric<T>::max_10_exp); }

                //! initial p value
                static inline int get_ini_exponent() throw() { return InitialExponent; }

                //! try to fit sample
                inline bool fit(SampleType<T>     &sample,
                                Sequential        &F,
                                Array             &aorg,
                                Array             &aerr,
                                const array<bool> &used,
                                Validate         *check=0)
                {
                    static const T  ftol = T(Y_LSF_FTOL);
                    static const T  zero(0);
                    static const T  one(1);

                    assert(aerr.size()==aorg.size());
                    assert(used.size()==aorg.size());
                    const size_t nvar  = aorg.size();
                    size_t       cycle = 0;             // number of cycles
                    size_t       nbad  = 0;             // number of successive bad steps

                    //__________________________________________________________
                    //
                    //
                    // initialize dynamic data and lambda
                    //
                    //__________________________________________________________
                    Y_LSF_OUT(std::cerr << "[LSF] \tinitializing for #variables=" << nvar << std::endl);
                    aerr.ld(0);        // no error
                    sample.prepare();  // internal memory and indexing

                    //----------------------------------------------------------
                    // check space size
                    //----------------------------------------------------------
                    if(nvar<=0)
                    {
                        Y_LSF_OUT(std::cerr << "[LSF] \t[- no variables -]" << std::endl);
                        return true;
                    }

                    //----------------------------------------------------------
                    // validate guess
                    //----------------------------------------------------------
                    if(check && !(*check)(aorg,used,sample.variables,cycle) )
                    {
                        Y_LSF_OUT(std::cerr << "[LSF] \t[- invalid first guess -]" << std::endl);
                        return false;
                    }

                    //----------------------------------------------------------
                    // acquire memory
                    //----------------------------------------------------------
                    this->acquire(nvar);   // internal arrays
                    alpha.make(nvar,nvar); // hessian
                    curv. make(nvar,nvar); // curvature

                    //----------------------------------------------------------
                    // initial step scaling
                    //----------------------------------------------------------
                    p   = get_ini_exponent();
                    compute_lam();
                    Y_LSF_OUT(std::cerr << "[LSF] \tstarting with lambda=" << lam << "/p=" << p << std::endl);

                    //----------------------------------------------------------
                    // one dimensional call
                    //----------------------------------------------------------
                    callD2     G = { &sample, &F, &aorg, &delta, &atry, 0 };

                    //----------------------------------------------------------
                    // full initial metrics
                    //----------------------------------------------------------
                    alpha.ld(zero);
                    beta.ld(zero);

                    Y_LSF_OUT(std::cerr << "[LSF] \tcomputing initial gradient..." << std::endl);
                    T        D2 = sample.computeD2(F,aorg,beta,alpha,grad,used);
                    Y_LSF_OUT(std::cerr << "[LSF] \tready for first cycle" << std::endl);

                    while(true)
                    {
                        ++cycle;
                        //______________________________________________________
                        //
                        //
                        // start cycle: normalize alpha and beta
                        //
                        //______________________________________________________
                        Y_LSF_OUT(std::cerr << "|" << std::endl;
                                  std::cerr << "[LSF] \t<cycle #" << cycle << ">" << std::endl;
                                  std::cerr << "[LSF] \t D2    = " << D2 << " @" << aorg << std::endl);
                        for(size_t i=nvar;i>0;--i)
                        {
                            Array &alpha_i = alpha[i];
                            if(used[i])
                            {
                                for(size_t j=i-1;j>0;--j)
                                {
                                    alpha[j][i] = alpha_i[j];
                                }
                            }
                            else
                            {
                                alpha_i[i] = one;  // for a null gradient
                                beta[i]    = zero; // mandatory
                            }
                        }
                        Y_LSF_OUT(std::cerr << "      \t beta  = "  << beta  << std::endl);
                        Y_LSF_OUT(std::cerr << "      \t alpha = "  << alpha << std::endl);

                        //______________________________________________________
                        //
                        //
                        // compute curvature if possible, using current lam
                        //
                        //______________________________________________________
                    FIND_STEP:
                        if( !compute_curvature() )
                        {
                            Y_LSF_OUT(std::cerr << "[LSF] \t[- singular parameters -]" << std::endl);
                            return false;
                        }

                        //______________________________________________________
                        //
                        // compute step from this point
                        //______________________________________________________
                        tao::set(delta,beta);
                        LU::solve(curv,delta);
                        Y_LSF_OUT(std::cerr << "      \t delta = " << delta << std::endl);

                        //______________________________________________________
                        //
                        // check acceptable step
                        //______________________________________________________
                        bool ok = true;
                        if(check)
                        {
                            tao::add(atry, aorg, delta);
                            if( !(*check)(atry,used,sample.variables,cycle) )
                            {
                                ok = false;
                                ++nbad;
                                //----------------------------------------------
                                // trial is not valid: decrease step after
                                // analysis
                                //----------------------------------------------
                                tao::sub(bound, atry, aorg);
                                Y_LSF_OUT(std::cerr << "      \t #bad  = " << nbad  << std::endl);
                                Y_LSF_OUT(std::cerr << "      \t bound = " << bound << std::endl);
                                T scale = one;
                                for(size_t i=nvar;i>0;--i)
                                {
                                    if(!used[i]) continue;
                                    const T dmax = fabs_of(bound[i]);
                                    const T dcur = fabs_of(delta[i]);
                                    if(dmax<dcur) scale = min_of(dmax/dcur,scale);
                                }
                                Y_LSF_OUT(std::cerr << "      \t scale@ini = " << scale << std::endl);
                                scale = min_of(scale,one)/2;
                                Y_LSF_OUT(std::cerr << "      \t scale@end = " << scale << std::endl);
                                tao::mulset(delta,scale,delta);
                            }
                        }
                        if(ok)
                        {
                            nbad = 0;
                        }


                        //______________________________________________________
                        //
                        // probe new value atry=aorg+delta at first
                        //______________________________________________________
                        G.calls  = 0;
                        T D2_try = G(one); assert(1==G.calls);
                        Y_LSF_OUT(std::cerr << "[LSF] \tD2_try = " << D2_try << "@" << atry << std::endl);

                        //______________________________________________________
                        //
                        // check if increasing D2
                        //______________________________________________________
                        if(D2_try>=D2)
                        {
                            Y_LSF_OUT(std::cerr << "[LSF] \tlocal extremum:" << std::endl);
                            
                            //__________________________________________________
                            //
                            // test variable convergence on INCREASING D2
                            //__________________________________________________
                            const T ferr = tao::fractional_error(aorg,atry);
                            Y_LSF_OUT(std::cerr << "[LSF] \terror on parameters: [ " << ferr << " ]" << std::endl);

                            if(ferr<=numeric<T>::sqrt_ftol)
                            {
                                Y_LSF_OUT(std::cerr << "[LSF] \t[- convergence of parameters -]" << std::endl);
                                goto CONVERGED;
                            }
                            //__________________________________________________
                            //
                            // will reduce step by increasing lambda if possible
                            //__________________________________________________
                            if(!increase_lambda())
                            {
                                Y_LSF_OUT(std::cerr << "[LSF] \t[- spurious parameters -]" << std::endl);
                                goto CONVERGED; // local minimum...
                            }
                            goto FIND_STEP;
                        }

                        //______________________________________________________
                        //
                        // successfull step: check numeric damping
                        //______________________________________________________
                        assert(D2_try<D2);
                        D2_try       = G.lookForDamping(D2, D2_try,verbose);
                        tao::set(aorg,atry);
                        const T    D2_err  = fabs_of(D2 - D2_try);
                        const bool lss_cvg = (D2_err <= ftol * D2);

                        Y_LSF_OUT(std::cerr << "[LSF] \tsquares   error : [ " << D2_err/D2 << " ]"       << std::endl);
                        Y_LSF_OUT(std::cerr << "[LSF]                     |_" << ConvergedText(lss_cvg) << std::endl);
                        Y_LSF_OUT(std::cerr << "[LSF]                     |_<  valid= " << YesOrNoText(ok)  << " >" << std::endl);

                        if( lss_cvg && ok )
                        {
                            goto CONVERGED;
                        }


                        //______________________________________________________
                        //
                        // prepare next step: full computation
                        //______________________________________________________
                        decrease_lambda();
                        alpha.ld(zero);
                        beta.ld(zero);
                        Y_LSF_OUT(std::cerr << "[LSF] \tupdating gradient..." << std::endl);
                        D2 = sample.computeD2(F,aorg,beta,alpha,grad,used); // will be D2_try
                        Y_LSF_OUT(std::cerr << "[LSF] \t<cycle #" << cycle << "/>" << std::endl);
                    }

                CONVERGED:
                    //__________________________________________________________
                    //
                    //
                    // D2_org, aorg and alpha are computed
                    //
                    //__________________________________________________________
                    Y_LSF_OUT(OutputLine(std::cerr << "|",Y_LSF_LINE) << std::endl);
                    Y_LSF_OUT(std::cerr << "[LSF] \tanalysis after #cycles="<< cycle << std::endl);

                    //__________________________________________________________
                    //
                    // compute the d.o.f
                    //__________________________________________________________
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
                        Y_LSF_OUT(std::cerr<< "[LSF] \tmore parameters than data" << std::endl);
                        aerr.ld(-one);
                        return false;
                    }
                    else if(nprm==ndat)
                    {
                        Y_LSF_OUT(std::cerr<< "[LSF] \tno degree of freedom: interpolation" << std::endl);
                        return true;
                    }
                    assert(ndat>nprm);
                    const size_t ndof = ndat-nprm;
                    Y_LSF_OUT(std::cerr << "    |_#dof        = " << ndof << std::endl);
                    Y_LSF_OUT(std::cerr << "    |_alpha=" << alpha << std::endl);

                    //__________________________________________________________
                    //
                    // compute the curvature matrix
                    //__________________________________________________________
                    if(!LU::build(alpha))
                    {
                        Y_LSF_OUT(std::cerr << "[LSF] unexpected singular minimum" << std::endl);
                        return false;
                    }

                    LU::inverse(alpha,curv);
                    Y_LSF_OUT(std::cerr<< "    |_curv=" << curv << std::endl);

                    //__________________________________________________________
                    //
                    // estimate the variance on parameters
                    //__________________________________________________________
                    const T dS = D2/ndof;
                    for(size_t i=nvar;i>0;--i)
                    {
                        if(used[i]) aerr[i] = sqrt_of( max_of(zero,dS*curv[i][i]) );
                    }
                    Y_LSF_OUT(std::cerr<< "    |_opt=" << aorg << std::endl);
                    Y_LSF_OUT(std::cerr<< "    |_err=" << aerr << std::endl);
                    Y_LSF_OUT(std::cerr<< "[LSF]"<<std::endl);
                    Y_LSF_OUT(OutputLine(std::cerr << "|",Y_LSF_LINE) << std::endl << std::endl);
                    return true;
                }

                //! wrapper to use regular function
                inline bool fit(SampleType<T>     &sample,
                                Function          &F,
                                Array             &aorg,
                                Array             &aerr,
                                const array<bool> &used,
                                Validate          *check=0)
                {
                    typename Type<T>::SequentialFunction SF(F);
                    return fit(sample,SF,aorg,aerr,used,check);
                }

                //! prototype to check positive used values
                bool CheckAllPositive( Array &aorg, const array<bool> &used, const Variables &vars, const size_t cycle )
                {
                    Y_LSF_OUT(std::cerr<< "[LSF] <check @cycle=" << cycle << ">" <<std::endl);
                    bool ans = true;
                    for( Variables::const_iterator i=vars.begin();i!=vars.end();++i)
                    {
                        const string &id = (**i).name;
                        if( vars(used,id) )
                        {
                            T &a = vars(aorg,id);
                            if(a<0)
                            {
                                Y_LSF_OUT(std::cerr<< "[LSF] [" << id << ": " << a << " -> 0" << "]" << std::endl);
                                a   = 0;
                                ans = false;
                            }
                        }
                    }
                    Y_LSF_OUT(std::cerr<< "[LSF] <check/>" <<std::endl);
                    return ans;
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

                //! increase lambda with overflow control
                bool increase_lambda() throw()
                {
                    static const int pmax = get_max_exponent();
                    if(++p>pmax) return false; //!< overflow
                    compute_lam();
                    Y_LSF_OUT(std::cerr << "[LSF] \t(+) lambda=" << lam  << std::endl);
                    return true;
                }

                //! decrease lambda with underflow control
                void decrease_lambda() throw()
                {
                    static const int pmin = get_min_exponent();

                    if(--p<pmin)
                    {
                        p   = pmin;
                        lam = 0;
                    }
                    else
                    {
                        compute_lam();
                    }
                    Y_LSF_OUT(std::cerr << "[LSF] \t(-) lambda="  << lam << std::endl);
                }

                //! compute curvature according to current lamnda
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


                //--------------------------------------------------------------
                //
                //! structure to get 1D function along descent step
                //
                //--------------------------------------------------------------
                struct callD2
                {
                    SampleType<T>  *sample;
                    Sequential     *seqfcn;
                    const Array    *aorg;
                    const Array    *delta;
                    Array          *atry;
                    unsigned        calls; //!< for one cycle

                    //! build a trial and get D2@atry
                    inline T operator()( const T u )
                    {
                        ++calls;
                        assert(sample); assert(seqfcn); assert(aorg); assert(delta); assert(atry);
                        tao::setprobe(*atry,*aorg,u,*delta);
                        return sample->computeD2(*seqfcn,*atry);
                    }

                    //! check if not too fast and slow down
                    inline T lookForDamping(const T    D2,
                                            const T    D2_try,
                                            const bool verbose)
                    {
                        static const T utol = T(Y_LSF_UTOL);
                        static const T uchk = T(1)-utol;

                        assert(1==calls);                       // reset in cycle
                        assert(D2_try<D2);                      // after a successfull step
                        callD2    &G = *this;                   // alias
                        triplet<T> u = { 0,  uchk,   1      };  assert(u.b>0);assert(u.b<1);
                        triplet<T> g = { D2, G(u.b), D2_try };
                        
                        if( g.b < D2_try )
                        {
                            Y_LSF_OUT(std::cerr << "[LSF] \t[ increasing => damping ]" << std::endl);
                            minimize::run(G,u,g,utol); // perform minimisation
                            const T ans = G(u.b);      // recompute final point
                            Y_LSF_OUT(std::cerr << "[LSF] \t\tdamped = " << u.b << std::endl);
                            Y_LSF_OUT(std::cerr << "[LSF] \t\tD2_try = " << g.b << ", #calls=" << calls << std::endl);
                            return ans;
                        }
                        else
                        {
                            Y_LSF_OUT(std::cerr << "[LSF] \t[ decreasing => forward ]" << std::endl);
                            return G(1); // recompute final point
                        }
                    }
                };

            };

        }
    }

}

#endif

