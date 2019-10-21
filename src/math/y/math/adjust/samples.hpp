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
                    Handles &self = *this;
                    for(size_t i=self.size();i>0;--i)
                    {
                        self[i]->ready();
                    }
                    deltaSq.adjust(self.size(),0);
                }

                Sample<T> & operator()( const Series &x, const Series &y,  Series &z )
                {
                    typename Sample<T>::Pointer tmp = new Sample<T>(x,y,z);
                    this->push_back(tmp);
                    return *tmp;
                }

                virtual T computeD2(Sequential<T> &F, const Array &aorg) const
                {
                    assert( deltaSq.size() == this->size() );

                    const Handles &self = *this;
                    for(size_t i=self.size();i>0;--i)
                    {
                        deltaSq[i] = self[i]->computeD2(F,aorg);
                    }
                    return sorted_sum(deltaSq);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Samples);
                mutable vector<mutable_type> deltaSq;
            };

        }

    }

}


#endif

