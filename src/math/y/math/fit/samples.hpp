//! \file
#ifndef Y_MATH_FIT_SAMPLES_INCLUDED
#define Y_MATH_FIT_SAMPLES_INCLUDED 1

#include "y/math/fit/variables.hpp"
#include "y/sequence/vector.hpp"
#include "y/math/kernel/lu.hpp"
#include "y/math/kernel/tao.hpp"
#include "y/math/fcn/derivative.hpp"

namespace upsylon
{
    namespace math
    {
        namespace Fit
        {
            ////////////////////////////////////////////////////////////////////
            //
            //! Common Sample information
            //
            ////////////////////////////////////////////////////////////////////
            class SampleInfo : public counted_object
            {
            public:
                Variables variables; //!< local or global variables
                virtual ~SampleInfo() throw();

            protected:
                explicit SampleInfo(const size_t nvar_max);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(SampleInfo);
            };

            ////////////////////////////////////////////////////////////////////
            //
            //! Type definitions
            //
            ////////////////////////////////////////////////////////////////////
            template <typename T>
            struct Type
            {
                typedef array<T>  Array;   //!< array interface
                typedef vector<T> Vector;  //!< sequence
                typedef matrix<T> Matrix;  //!< matrix
                typedef functor<T,TL3(T,const Array&,const Variables&)> Function; //!< fit function prototype!

                //! compute gradient of fit function
                class Gradient : public derivative<T>
                {
                public:
                    T scale; //!< parameters scaling, default=1e-4
                    inline explicit Gradient() : derivative<T>(), scale(1e-4) {}
                    inline virtual ~Gradient() throw() {}

                    inline void operator()(Array             &dFda,
                                           Function          &F,
                                           const T            x,
                                           const Array       &aorg,
                                           const Variables   &vars,
                                           const array<bool> &used)
                    {
                        assert(dFda.size()==aorg.size());
                        assert(used.size()==aorg.size());
                        Parameters proxy = { x, *aorg, &vars, &F, 0 };
                        size_t    &i     = proxy.ivar;
                        for(i=aorg.size();i>0;--i)
                        {
                            if(used[i])
                            {
                                dFda[i] = this->diff(proxy,aorg[i],scale);
                            }
                            else
                            {
                                dFda[i] = 0;
                            }
                        }
                    }

                private:
                    struct Parameters
                    {
                        T                xvalue;
                        const Array     *aorg_p;
                        const Variables *vars_p;
                        Function        *func_p;
                        size_t           ivar;
                        inline T operator()( T atry )
                        {
                            assert(aorg_p); assert(vars_p); assert(func_p);
                            assert(ivar>0);
                            assert(ivar<=aorg_p->size());
                            Array           &a = (Array &)(*aorg_p);
                            const Variables &v = *vars_p;
                            Function        &F = *func_p;
                            T               &ai = a[ivar];
                            const T          a0 = ai;
                            try
                            {
                                ai = atry;
                                const T ans = func(xvalue,a,v);
                                ai = a0;
                                return ans;
                            }
                            catch(...)
                            {
                                a[ivar] = a0; throw;
                            }
                        }
                    };
                };
            };



            ////////////////////////////////////////////////////////////////////
            //
            //! Interface to sample with local data
            //
            ////////////////////////////////////////////////////////////////////
            template <typename T>
            class SampleType : public SampleInfo
            {
            public:
                typedef typename Type<T>::Function Function;
                typedef typename Type<T>::Array    Array;
                typedef typename Type<T>::Matrix   Matrix;

                //! desctructor
                inline virtual ~SampleType() throw() {}

                //! compute D2 only
                virtual T computeD2(Function     &F,
                                    const Array  &aorg) = 0;

            protected:
                inline explicit SampleType(const size_t nvar_max) :
                SampleInfo(nvar_max)
                {
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(SampleType);
            };


            ////////////////////////////////////////////////////////////////////
            //
            //! one sample
            //
            ////////////////////////////////////////////////////////////////////
            template <typename T>
            class Sample : public SampleType<T>
            {
            public:
                typedef arc_ptr<Sample> Pointer;
                typedef vector<Pointer> Collection;

                typedef typename Type<T>::Function Function;
                typedef typename Type<T>::Array    Array;
                typedef typename Type<T>::Matrix   Matrix;

                const Array &X;  //!< X values
                const Array &Y;  //!< Y values
                Array       &Yf; //!< Yf = F(x,...) values

                inline virtual ~Sample() throw() {}
                inline explicit Sample(const Array &userX,
                                       const Array &userY,
                                       Array       &userYf,
                                       const size_t nvar_max=0) :
                SampleType<T>(nvar_max),
                X(userX),
                Y(userY),
                Yf(userYf)
                {}



                virtual T computeD2(Function     &F,
                                    const Array  &aorg)
                {
                    assert(X.size()==Y.size());
                    assert(X.size()==Yf.size());
                    T ans = 0;
                    for(size_t i=X.size();i>0;--i)
                    {
                        const T Fi = (Yf[i]=F(X[i],aorg,this->variables));
                        ans += square_of(Fi);
                    }
                    return ans;
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Sample);
            };

            ////////////////////////////////////////////////////////////////////
            //
            //! multiple samples
            //
            ////////////////////////////////////////////////////////////////////
            template <typename T>
            class Samples : public SampleType<T>, public Sample<T>::Collection
            {
            public:
                typedef typename Type<T>::Function  Function;
                typedef typename Type<T>::Array     Array;
                typedef typename Type<T>::Matrix    Matrix;
                typedef typename Sample<T>::Pointer Pointer;

                inline virtual ~Samples() throw() {}
                inline explicit Samples(const size_t nvar_max=0,const size_t size_max=0) : SampleType<T>(nvar_max), Sample<T>::Collection(size_max,as_capacity) {}

                virtual T computeD2(Function     &F,
                                    const Array  &aorg)
                {
                    typename Sample<T>::Collection &self = *this;
                    T ans = 0;
                    for(size_t k=this->size();k>0;--k)
                    {
                        ans += self[k]->computeD2(F,aorg);
                    }
                    return ans;
                }

                //! create a append a new sample
                Sample<T> & add(const Array &userX,
                                const Array &userY,
                                Array       &userYf,
                                const size_t nvar_max=0)
                {
                    Pointer p = new Sample<T>(userX,userY,userYf,nvar_max);
                    this->push_back(p);
                    return *p;
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Samples);
            };

        }
    }
}

#endif

