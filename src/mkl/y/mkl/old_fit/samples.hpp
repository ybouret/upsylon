//! \file
#ifndef Y_MATH_FIT_SAMPLES_INCLUDED
#define Y_MATH_FIT_SAMPLES_INCLUDED 1

#include "y/math/fit/sample.hpp"

namespace upsylon
{
    namespace math
    {
        namespace Fit
        {

           
            ////////////////////////////////////////////////////////////////////
            //
            //! multiple samples
            //
            ////////////////////////////////////////////////////////////////////
            template <typename T>
            class Samples : public SampleType<T>, public Sample<T>::Collection
            {
            public:
                typedef typename Sample<T>::Collection SelfType;   //!< alias
                typedef typename Type<T>::Function     Function;   //!< alias
                typedef typename Type<T>::Sequential   Sequential; //!< alias
                typedef typename Type<T>::Array        Array;      //!< alias
                typedef typename Type<T>::Matrix       Matrix;     //!< alias
                typedef typename Sample<T>::Pointer    Pointer;    //!< alias
                typedef typename Type<T>::Gradient     Gradient;   //!< alias

                //! destructor
                inline virtual ~Samples() throw() {}

                //! initialize
                inline explicit Samples(const size_t nvar_max=0,const size_t size_max=0) : SampleType<T>(nvar_max), SelfType(size_max,as_capacity) {}

                //! gather D2 weigthed values
                virtual T computeD2(typename Type<T>::Sequential &F,
                                    const Array                  &aorg)
                {
                    SelfType    &self = *this;
                    const size_t n    = this->size();
                    this->rc.free(); assert(this->rc.capacity()>=n);
                    for(size_t k=n;k>0;--k)
                    {
                        this->rc.push_back_( self[k]->computeD2(F,aorg) );
                    }
                    return sorted_sum(this->rc);
                }

                //! sum of all counts
                virtual size_t count() const throw()
                {
                    const SelfType &self = *this;
                    size_t          ans  = 0;
                    for(size_t k=self.size();k>0;--k)
                    {
                        ans += self[k]->count();
                    }
                    return ans;
                }


                //! prepare all samples and self resources
                virtual void prepare()
                {
                    typename Sample<T>::Collection &self = *this;
                    const size_t n = self.size();
                    this->rc.free();
                    this->rc.ensure(n);
                    for(size_t k=n;k>0;--k)
                    {
                        self[k]->prepare();
                    }
                }

                //! gather from samples
                virtual inline void add_SSE_SSR( T &SSE, T &SSR ) const
                {
                    const SelfType &self = *this;
                    for(size_t k=self.size();k>0;--k)
                    {
                        self[k]->add_SSE_SSR(SSE,SSR);
                    }
                }

                //! gather from all samples and get global correlation
                virtual T compute_correlation( correlation<T> &corr ) const
                {
                    const SelfType &self = *this;
                    corr.free();
                    for(size_t k=self.size();k>0;--k)
                    {
                        const Sample<T> &sample = *self[k];
                        for(size_t i=sample.X.size();i>0;--i)
                        {
                            corr.add( sample.Y[i], sample.Yf[i] );
                        }
                    }
                    return corr.compute();
                }

                //! create and append a new sample
                Sample<T> & add(const Array &userX,
                                const Array &userY,
                                Array       &userYf,
                                const size_t nvar_max=0)
                {
                    Pointer p = new Sample<T>(userX,userY,userYf,nvar_max);
                    this->push_back(p);
                    return *p;
                }

                //! compute D2 and sum all differential values
                virtual T computeD2(Sequential       &F,
                                    const Array       &aorg,
                                    Array             &beta,
                                    Matrix            &alpha,
                                    Gradient          &grad,
                                    const array<bool> &used)
                {
                    SelfType    &self = *this;
                    const size_t n    = self.size();
                    
                    this->rc.free(); assert(this->rc.capacity()>=n);
                    for(size_t k=n;k>0;--k)
                    {
                        this->rc.push_back_( self[k]->computeD2(F,aorg,beta,alpha,grad,used) );
                    }
                    return sorted_sum(this->rc);
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Samples);
            };

        }
    }
}

#endif

