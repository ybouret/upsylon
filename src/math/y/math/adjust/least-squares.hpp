//! \file

#ifndef Y_ADJUST_LEAST_SQUARES_INCLUDED
#define Y_ADJUST_LEAST_SQUARES_INCLUDED 1

#include "y/math/adjust/sample/type.hpp"
#include "y/math/kernel/lu.hpp"
#include "y/oxide/field1d.hpp"

namespace upsylon {
    
    namespace math {
        
        namespace Adjust {

            //==================================================================
            //
            //
            //! utilities for LeastSquares
            //
            //
            //==================================================================
            template <typename T> struct Algo
            {
                typedef typename Type<T>::Matrix   Matrix;
                typedef typename Type<T>::Array    Array;
                
                static inline unit_t MinPower() throw()
                {
                    return -unit_t(numeric<T>::dig)-1;
                }
                
                static inline unit_t MaxPower() throw()
                {
                    return unit_t(numeric<T>::max_10_exp);
                }

                static inline unit_t Initial() throw()
                {
                    return -4;
                }
                
                
                
                
                static inline
                bool ComputeCurvature(Matrix       &curv,
                                      const T       lambda,
                                      const Matrix &alpha) throw()
                {
                    assert(lambda>=0);
                    assert(alpha.is_square);
                    assert(curv.same_size_than(alpha));
                    
                    static const T one = 1;
                    const T        fac = one + lambda;
                    const size_t   n   = alpha.rows;
                    
                    for(size_t i=n;i>0;--i)
                    {
                        addressable<T>  &curv_i = curv[i];
                        size_t j=n;
                        for(;j>i;--j)
                        {
                            curv_i[j] = alpha[i][j];
                        }
                        assert(j==i);
                        curv_i[i] = fac * alpha[i][i];
                        for(--j;j>0;--j)
                        {
                            curv_i[j] = alpha[i][j];
                        }
                    }
                    return LU::build(curv);
                }
                
                static inline
                void ComputeStep(Array                &step,
                                 const Matrix         &curv,
                                 const accessible<T>  &beta) throw()
                {
                    atom::set(step,beta);
                    LU::solve(curv,step);
                }
                
                
            };


#define Y_LS_PRINTLN(OUTPUT) do{ if(verbose) { std::cerr << OUTPUT << std::endl; } } while(false)

            //==================================================================
            //
            //
            //! LeastSquares
            //
            //
            //==================================================================
            template <typename T>
            class LeastSquares : public Gradient<T>
            {
            public:
                typedef typename Type<T>::Array    Array;
                typedef typename Type<T>::Matrix   Matrix;
                typedef typename Type<T>::Vector   Vector;
                typedef Oxide::Field1D<T>          Field;
                
                inline explicit LeastSquares(const bool verb=false) :
                verbose(verb),
                lambdas("lambda",Algo<T>::MinPower(),Algo<T>::MaxPower()),
                pmin(lambdas.lower),
                pmax(lambdas.upper),
                p(0),
                lambda(0),
                alpha(),
                beta(),
                curv(),
                step(),
                atry(),
                used()
                {
                    setup();
                }
                
                inline virtual ~LeastSquares() throw() {}
                
                bool         verbose;
                const Field  lambdas;
                const unit_t pmin;
                const unit_t pmax;
                
                
                bool fit(SampleType<T> &sample,
                         Sequential<T> &F,
                         Array         &aorg,
                         const Flags   &flags,
                         Array         &aerr)
                {
                    assert( flags.size() == aorg.size() );
                    assert( flags.size() == aerr.size() );

                    //__________________________________________________________
                    //
                    Y_LS_PRINTLN( "[LS] initializing..." );
                    //__________________________________________________________
                    sample.ready();
                    atom::ld(aerr,-1);
                    const size_t n = aorg.size();
                    if(n<=0)
                    {
                        Y_LS_PRINTLN("[LS] no parameters");
                        return true;
                    }

                    //__________________________________________________________
                    //
                    Y_LS_PRINTLN( "[LS] starting with #parameters=" << n);
                    //__________________________________________________________
                    alpha.make(n,n);
                    beta.adjust(n,0);
                    curv.make(n,n);
                    step.adjust(n,0);
                    atry.adjust(n,0);
                    used.adjust(n,0);
                    atom::ld(used,false);
                    sample.activate(used,flags);
                    Y_LS_PRINTLN( "     flags  = " << flags );
                    Y_LS_PRINTLN( "     used   = " << used  );


                    setLambda( Algo<T>::Initial() );
                    CallD2 D2    = { &aorg, &step, &atry, &sample, &F };
                    T      D2org = sample.computeD2(alpha, beta, F, aorg, used, *this);
                    size_t cycle = 0;


                CYCLE:
                    ++cycle;
                    //__________________________________________________________
                    //
                    Y_LS_PRINTLN( "[LS] cycle  = " << cycle );
                    Y_LS_PRINTLN( "     aorg   = " << aorg );
                    Y_LS_PRINTLN( "     D2org  = " << D2org );
                    Y_LS_PRINTLN( "     alpha  = " << alpha );
                    //__________________________________________________________

                CURVATURE:
                    //__________________________________________________________
                    //
                    Y_LS_PRINTLN( "     beta   = " << beta  );
                    while( !Algo<T>::ComputeCurvature(curv,lambda,alpha) )
                    {
                        if(!increaseLambda())
                        {
                            Y_LS_PRINTLN( "[LS] singular point" );
                            return false;
                        }
                    }
                    //__________________________________________________________

                    //__________________________________________________________
                    //
                    Algo<T>::ComputeStep(step, curv, beta);
                    Y_LS_PRINTLN( "     lambda = " << lambda );
                    Y_LS_PRINTLN( "     step   = " << step   );
                    //__________________________________________________________

                    
                    // possible step modification

                    //__________________________________________________________
                    //
                    // try full step
                    T D2try = D2(1);
                    Y_LS_PRINTLN( "     atry   = " << atry   );
                    Y_LS_PRINTLN( "     D2try  = " << D2try  );
                    //__________________________________________________________


                    if( D2try < D2org )
                    {
                        //______________________________________________________
                        //
                        Y_LS_PRINTLN( "[LS] accept" );
                        //______________________________________________________
                        const bool  converged = ( fabs_of(D2org-D2try) <= numeric<T>::sqrt_ftol * D2org);
                        decreaseLambda();
                        atom::set(aorg,atry);
                        D2org = sample.computeD2(alpha, beta, F, aorg, used, *this);
                        if(converged)
                        {
                            goto CONVERGED;
                        }

                        goto CYCLE;
                    }
                    else
                    {
                        //______________________________________________________
                        //
                        Y_LS_PRINTLN( "[LS] reject" );
                        //______________________________________________________
                        if( !increaseLambda() )
                        {
                            Y_LS_PRINTLN( "[LS] cannot move..." );
                            return false;
                        }
                        goto CURVATURE;

                    }

                CONVERGED:
                    //__________________________________________________________
                    //
                    Y_LS_PRINTLN( "[LS] converged@D2=" << D2org );
                    Y_LS_PRINTLN( "     aorg = " << aorg );
                    //__________________________________________________________
                    if(!LU::build(alpha))
                    {
                        Y_LS_PRINTLN( "[LS] singular extremum" );
                        return false;
                    }
                    LU::inverse(alpha,curv);

                    //__________________________________________________________
                    //
                    unit_t dof = sample.count();
                    //__________________________________________________________
                    for(size_t i=n;i>0;--i)
                    {
                        if(used[i]) --dof;
                    }
                    Y_LS_PRINTLN( "      dof = " << dof );
                    if(dof<0)
                    {
                        Y_LS_PRINTLN( "[LS] meaningless result" );
                        return false;
                    }
                    else if(0==dof)
                    {
                        Y_LS_PRINTLN( "[LS] is interpolation" );
                        atom::ld(aerr,0);
                        return true;
                    }
                    else
                    {
                        assert(dof>0);
                        const T real_count = T( sample.count() );
                        const T sig2err    = T(1)/sqrt_of(real_count);
                        for(size_t i=n;i>0;--i)
                        {
                            if(used[i])
                            {
                                const T sig = sqrt_of( (D2org * curv[i][i]) / dof );
                                aerr[i] = sig*sig2err;
                            }
                            else
                            {
                                aerr[i] = 0;
                            }
                        }
                        Y_LS_PRINTLN( "     aerr = " << aerr );
                        return true;
                    }



                }
                
                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(LeastSquares);
                unit_t p;
                T      lambda;
                
                Matrix   alpha;
                Vector   beta;
                Matrix   curv;
                Vector   step;
                Vector   atry;
                bVector  used;

                struct CallD2
                {
                    const accessible<T> * _aorg;
                    const accessible<T> * _step;
                    addressable<T>      * _atry;
                    const SampleType<T> * _sample;
                    Sequential<T>         * _F;
                    
                    inline T operator()( const T u )
                    {
                        atom::setprobe(* _atry, * _aorg, u, *_step);
                        return _sample->computeD2( *_F, * _atry);
                    }
                    
                    
                };
                
                
                inline void setup() throw()
                {
                    Field &lam = aliasing::_(lambdas);
                    lam[ pmin ] = 0;
                    {
                        static const T tenth(0.1);
                        lam[ -1 ]        = tenth;
                        for(unit_t i=-2;i>pmin;--i) lam[i] = lam[i+1] * tenth;
                    }
                    lam[  0 ]            = 1;
                    {
                        static const T ten(10);
                        lam[  1 ]            = ten;
                        for(unit_t i=2;i<=pmax;++i) lam[i] = lam[i-1] * ten;
                    }
                    setLambda(0);
                }
                
                inline void setLambda( const unit_t p0 ) throw()
                {
                    lambda = lambdas[ (p=clamp(pmin,p0,pmax) ) ];
                }
                
                inline void decreaseLambda() throw()
                {
                    lambda = lambdas[ (p=max_of(p-1,pmin) ) ];
                    Y_LS_PRINTLN( "[LS] decreasing lambda" );
                }
                
                inline bool increaseLambda() throw()
                {
                    Y_LS_PRINTLN( "[LS] increasing lambda" );
                    if(p>=pmax)
                    {
                        Y_LS_PRINTLN( "   |_<overflow>" );
                        return false;
                    }
                    else
                    {
                        lambda = lambdas[ ++p ];
                        return true;
                    }
                }
                
            };
            
        }
    }
}


#endif

