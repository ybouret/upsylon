//! \file

#ifndef Y_MATH_ADJUST_SAMPLESINCLUDED
#define Y_MATH_ADJUST_SAMPLES_INCLUDED 1

#include "y/math/adjust/sample.hpp"

namespace upsylon {

    namespace math {

        namespace Adjust {

            template <typename T>
            class Samples : public SampleType<T>, public Sample<T>::Handles
            {
            public:
                Y_DECL_ARGS(T,type);

                typedef typename Sample<type>::Handles Handles;
                typedef typename Type<type>::Series    Series;
                typedef typename Type<type>::Array     Array;
                typedef typename Type<type>::Matrix    Matrix;

                inline virtual ~Samples() throw()
                {
                }

                inline explicit Samples() throw() :
                SampleType<T>(), Handles(), deltaSq()
                {
                }

                inline virtual size_t count() const throw()
                {
                    const Handles &self = *this;
                    size_t         ans  = 0;
                    for(size_t i=self.size();i>0;--i)
                    {
                        ans += self[i]->count();
                    }
                    return ans;
                }

                inline virtual void ready()
                {
                    Handles     &self = *this;
                    const size_t ns   = self.size();
                    deltaSq.adjust(ns,0);
                    weights.adjust(ns,0);
                    for(size_t i=ns;i>0;--i)
                    {
                        self[i]->ready();
                        weights[i] = 1;
                    }
                  
                }

                Sample<T> & operator()( const Series &x, const Series &y,  Series &z )
                {
                    typename Sample<T>::Pointer tmp = new Sample<T>(x,y,z);
                    this->push_back(tmp);
                    return *tmp;
                }

                virtual T compute(Sequential<T> &F, const Array &aorg) const
                {
                    assert( deltaSq.size() == this->size() );

                    const Handles &self = *this;
                    for(size_t i=self.size();i>0;--i)
                    {
                        deltaSq[i] = weights[i] * self[i]->compute(F,aorg);
                    }
                    return sorted_sum(deltaSq);
                }
                
                virtual T  computeAndUpdate(Matrix          &alpha,
                                            Array           &beta,
                                            Sequential<T>   &F,
                                            const Array     &aorg,
                                            const Flags     &used,
                                            Gradient<T>     &grad) const
                {
                    const Handles &self = *this;
                    const size_t   n    = used.size();
                    _alpha.make(n,n);
                    _beta.adjust(n, 0);
                    
                    for(size_t i=self.size();i>0;--i)
                    {
                        _alpha.ld(0);
                        atom::ld(_beta,0);
                        const_type w = weights[i];
                        deltaSq[i]   = w * self[i]->computeAndUpdate(_alpha,_beta,F,aorg,used,grad);
                        for(size_t j=n;j>0;--j)
                        {
                            beta[j] += w * _beta[j];
                            for(size_t k=j;k>0;--k)
                            {
                                alpha[j][k] += w * _alpha[j][k];
                            }
                        }
                    }
                    return sorted_sum(deltaSq);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Samples);
                mutable vector<mutable_type> deltaSq;
                mutable vector<mutable_type> weights;
                mutable matrix<mutable_type> _alpha;
                mutable vector<mutable_type> _beta;
                
            };

        }

    }

}


#endif

