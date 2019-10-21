//! \file

#ifndef Y_ADJUST_LEAST_SQUARES_INCLUDED
#define Y_ADJUST_LEAST_SQUARES_INCLUDED 1

#include "y/math/adjust/sample/type.hpp"
#include "y/math/kernel/lu.hpp"
#include "y/oxide/field1d.hpp"

namespace upsylon {
    
    namespace math {
        
        namespace Adjust {
            
            template <typename T>
            struct Algo
            {
                typedef typename Type<T>::Matrix   Matrix;
                typedef typename Type<T>::Array    Array;
                
                static inline unit_t MinPower() throw()
                {
                    return -unit_t(numeric<T>::dig)-1;
                }
                
                static inline int MaxPower() throw()
                {
                    return unit_t(numeric<T>::max_10_exp);
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
                    std::cerr << "alpha  =" << alpha << std::endl;
                    std::cerr << "alpha_l=" << curv << std::endl;
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
                lambda(0)
                {
                    setup();
                }
                
                inline virtual ~LeastSquares() throw() {}
                
                bool         verbose;
                const Field  lambdas;
                const unit_t pmin;
                const unit_t pmax;
                
                
                bool fit( SampleType<T>       &sample,
                         Sequential<T>        &F,
                         Array                &aorg,
                         const Flags          &used )
                {
                    assert( used.size() == aorg.size() );
                    
                    sample.ready();
                    const size_t n = aorg.size();
                    if(n<=0)
                    {
                        return true;
                    }
                    
                    alpha.make(n,n);
                    beta.adjust(n,0);
                    curv.make(n,n);
                    step.adjust(n,0);
                    atry.adjust(n,0);
                    
                    CallD2 D2 = { &aorg, &step, &atry, &sample, &F };
                    
                    setLambda(0);
                    
                    // initialize
                    T D2org = sample.computeD2(alpha, beta, F, aorg, used, *this);
                    std::cerr << "D2org=" << D2org<< std::endl;
                    
                    std::cerr << "alpha=" << alpha << std::endl;
                    std::cerr << "beta =" << beta  << std::endl;
                    // compute curvature at current point
                    while( !Algo<T>::ComputeCurvature(curv,lambda,alpha))
                    {
                        if(!increaseLambda())
                        {
                            return false;
                        }
                    }
                    std::cerr << "lambda=" << lambda << std::endl;
                    
                    // compute current 'optimal' step
                    Algo<T>::ComputeStep(step, curv, beta);
                    std::cerr << "step=" << step << std::endl;
                    
                    
                    // possible step modification
                    
                    // try full step
                    T D2try = D2(1);
                    aorg.display(std::cerr << "aorg=")  << std::endl;
                    std::cerr << "atry=" << atry << std::endl;

                    
                    std::cerr << "D2try=" << D2try << std::endl;
                    
                    
                    return true;
                    
                }
                
                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(LeastSquares);
                unit_t p;
                T      lambda;
                
                Matrix alpha;
                Vector beta;
                Matrix curv;
                Vector step;
                Vector atry;
                
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
                    aliasing::_(lambda) = lambdas[ (aliasing::_(p)=clamp(pmin,p0,pmax) ) ];
                }
                
                inline void decreaseLambda() throw()
                {
                    lambda = lambdas[ (p=max_of(p-1,pmin) ) ];
                }
                
                inline bool increaseLambda() throw()
                {
                    if(p>=pmax)
                    {
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

