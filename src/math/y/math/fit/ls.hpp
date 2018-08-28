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
                Gradient grad;
                bool     verbose;

                inline explicit LeastSquare() :
                arrays<T>(10),
                p(0),
                lam(0),
                alpha(),
                beta(  this->next() ),
                delta( this->next() ),
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
                    const size_t n = alpha.rows;
                    while(true)
                    {
                        curv.assign( alpha );
                        const T      fac = T(1) + lam;
                        for(size_t i=n;i>0;--i)
                        {
                            curv[i][i] *= fac;
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

                    if(nvar<=0) return true;

                    this->acquire(nvar);
                    alpha.make(nvar,nvar);
                    curv.make(nvar,nvar);

                    // init
                    aerr.ld(0);
                    alpha.ld(0);
                    beta.ld(0);
                    p   = -4;
                    compute_lam();
                    T D2_org = sample.computeD2(F,aorg,beta,alpha,grad,used);
                    while(true)
                    {
                        //______________________________________________________
                        //
                        // normalize alpha
                        //______________________________________________________
                        if(verbose) { std::cerr << "[LSF] D2_org=" << D2_org << std::endl; }

                        std::cerr << "beta=" << beta << std::endl;
                        std::cerr << "alpha=" << alpha << std::endl;
                        exit(1);
                        if( !compute_curvature() )
                        {
                            if(verbose) { std::cerr << "[LSF] Singular Point" << std::endl; }
                            return false;
                        }
                        break;
                    }


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
                            return lam = 1;
                        }
                    }
                }
            };

        }
    }

}

#endif

