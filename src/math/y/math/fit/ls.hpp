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

                Matrix   alpha;
                Matrix   curv;
                Array   &beta;
                Array   &delta;
                Gradient grad;
                bool     verbose;

                inline explicit LeastSquare() :
                arrays<T>(10),
                alpha(),
                beta(  this->next() ),
                delta( this->next() ),
                grad(),
                verbose(true)
                {
                }

                inline virtual ~LeastSquare() throw()
                {
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
                    T D2_org = sample.computeD2(F,aorg,beta,alpha,grad,used);
                    while(true)
                    {
                        if(verbose) { std::cerr << "[LSF] D2_org=" << D2_org << std::endl; }

                        break;
                    }


                    return false;
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(LeastSquare);

            };

        }
    }

}

#endif

