//! \file

#ifndef Y_MATH_ADJUST_SAMPLES_INCLUDED
#define Y_MATH_ADJUST_SAMPLES_INCLUDED 1

#include "y/math/adjust/sample.hpp"

namespace upsylon {

    namespace math {

        namespace Adjust {

            //==================================================================
            //
            //
            //! multiple weighted samples
            //
            //
            //==================================================================
            template <typename T>
            class Samples : public SampleType<T>, public Sample<T>::Handles
            {
            public:
                //==============================================================
                //
                // types and definitions
                //
                //==============================================================
                typedef typename Sample<T>::Handles Handles; //!< alias
                typedef typename Type<T>::Series    Series;  //!< alias
                typedef typename Type<T>::Array     Array;   //!< alias
                typedef typename Type<T>::Matrix    Matrix;  //!< alias


                //==============================================================
                //
                // virtual interface
                //
                //==============================================================

                //! cleanup
                inline virtual ~Samples() throw()
                {
                }


                //! sum of all counts
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

                //! ready all samples and compute weights
                inline virtual void ready()
                {
                    Handles     &self = *this;
                    const size_t ns   = self.size();
                    deltaSq.adjust(ns,0);
                    weights.adjust(ns,0);
                    size_t total_w = 0;
                    for(size_t i=ns;i>0;--i)
                    {
                        self[i]->ready();
                        const size_t w = self[i]->count();
                        total_w   += w;
                        weights[i] = w;
                    }

                    for(size_t i=ns;i>0;--i)
                    {
                        weights[i] /= total_w;
                    }
                  
                }

                //! return a weighted D2
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

                //! return weighted alpha, beta and D2
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
                        const T  w = weights[i];
                        deltaSq[i] = w * self[i]->computeAndUpdate(_alpha,_beta,F,aorg,used,grad);
                        
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

                //! activate all matching flags of all samples
                virtual void activate( addressable<bool> &target, const accessible<bool> &source ) const
                {
                    const Handles &self = *this;
                    for(size_t i=self.size();i>0;--i)
                    {
                        self[i]->activate(target,source);
                    }
                }

                //==============================================================
                //
                // non virtual interfaces
                //
                //==============================================================

                //! setup
                inline explicit Samples() throw() :
                SampleType<T>(), Handles(), deltaSq(), weights(), _alpha(), _beta()
                {

                }

                //! register a new sample
                Sample<T> & operator()( const Series &x, const Series &y,  Series &z )
                {
                    typename Sample<T>::Pointer tmp = new Sample<T>(x,y,z);
                    this->push_back(tmp);
                    return *tmp;
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Samples);
                mutable vector<T> deltaSq;
                vector<T>         weights;
                mutable matrix<T> _alpha;
                mutable vector<T> _beta;
                
            };

        }

    }

}


#endif

