//! \file
#ifndef Y_MATH_FIT_SAMPLES_INCLUDED
#define Y_MATH_FIT_SAMPLES_INCLUDED 1

#include "y/math/fit/variables.hpp"
#include "y/sequence/vector.hpp"
#include "y/math/kernel/lu.hpp"
#include "y/math/kernel/tao.hpp"
#include "y/math/fcn/derivative.hpp"
#include "y/math/stat/metrics.hpp"
#include "y/sort/sorted-sum.hpp"
#include "y/sort/index.hpp"

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
                //______________________________________________________________
                //
                // types
                //______________________________________________________________
                typedef   vector<size_t> Indices; //!< indices for ordered evaluations

                //______________________________________________________________
                //
                // virtual interface
                //______________________________________________________________
                virtual size_t count() const throw() = 0; //!< number of points
                virtual void   prepare()             = 0; //!< prepare resources

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                Variables variables; //!< local or global variables

            protected: explicit SampleInfo(const size_t nvar_max);  //!< initialize
            public:    virtual ~SampleInfo() throw();               //!< destructor

            private:
                Y_DISABLE_COPY_AND_ASSIGN(SampleInfo);
            };

            ////////////////////////////////////////////////////////////////////
            //
            //! Type dependent definitions
            //
            ////////////////////////////////////////////////////////////////////
            template <typename T>
            struct Type
            {
                //______________________________________________________________
                //
                // aliases
                //______________________________________________________________
                typedef array<T>  Array;   //!< array interface
                typedef vector<T> Vector;  //!< sequence
                typedef matrix<T> Matrix;  //!< matrix
                typedef functor<T,TL3(T,const Array&,const Variables&)> Function; //!< fit function  prototype
                typedef T (*CFunction)(T,const Array&,const Variables&);          //!< fit CFunction prototype

                //______________________________________________________________
                //
                // Sequential Call
                //______________________________________________________________

                //! interface for sequent call
                class Sequential
                {
                public:
                    const T current; //!< currently called position
                    inline virtual ~Sequential() throw() { (T&)current=0; } //!< cleanup

                    //! make a first call
                    inline T initialize(T x, const Array &aorg, const Variables &vars)
                    {
                        const T  ans = on_initialize(x,aorg,vars);
                        (T &)current = x;
                        return ans;
                    }

                    //! update value from previous call
                    inline T compute_to(T x, const Array &aorg, const Variables &vars)
                    {
                        const T  ans = on_compute_to(x,aorg,vars);
                        (T &)current = x;
                        return ans;
                    }


                protected:
                    //! setup
                    inline explicit Sequential() throw() : current(0)
                    {
                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Sequential);
                    virtual T on_initialize(T, const Array &,const Variables &) = 0;
                    virtual T on_compute_to(T, const Array &,const Variables &) = 0;
                };

                //! lightweight proxy for regular function
                class SequentialFunction : public Sequential
                {
                public:
                    inline explicit SequentialFunction( Function &F ) throw() : host(F) {} //!< setup
                    inline virtual ~SequentialFunction() throw() {}                        //!< cleanup
                    Function &host; //!< reference to external function

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(SequentialFunction);
                    inline virtual T on_initialize(T x, const Array &aorg, const Variables &vars) { return host(x,aorg,vars); }
                    inline virtual T on_compute_to(T x, const Array &aorg, const Variables &vars) { return host(x,aorg,vars); }

                };


                //! compute gradient of fit function
                class Gradient : public derivative<T>
                {
                public:
                    typedef typename Type<T>::Sequential Sequential; //!< alias

                    T scale; //!< parameters scaling, default=1e-4

                    //! initialize
                    inline explicit Gradient() : derivative<T>(), scale(1e-4) {}
                    //! destuctor
                    inline virtual ~Gradient() throw() {}

                    //! compute all active components
                    inline void operator()(Array              &dFda,
                                           Sequential         &F,
                                           const T             x,
                                           const Array        &aorg,
                                           const Variables    &vars,
                                           const array<bool>  &used)
                    {
                        assert(dFda.size()==aorg.size());
                        assert(used.size()==aorg.size());
                        Parameters proxy = { x, &aorg, &vars, &F, 0 };
                        size_t    &i     = proxy.ivar;
                        for(i=aorg.size();i>0;--i)
                        {
                            const size_t ii = i;
                            if(used[ii])
                            {
                                dFda[ii] = this->diff(proxy,aorg[ii],scale);
                            }
                            else
                            {
                                dFda[ii] = 0;
                            }
                        }
                    }

                private:
                    struct Parameters
                    {
                        T                xvalue;
                        const Array     *aorg_p;
                        const Variables *vars_p;
                        Sequential      *func_p;
                        size_t           ivar;
                        inline T operator()( T atry )
                        {
                            assert(aorg_p); assert(vars_p); assert(func_p);
                            assert(ivar>0); assert(ivar<=aorg_p->size());

                            Array           &a  = (Array &)(*aorg_p);
                            T               &ai = a[ivar];
                            const T          a0 = ai;
                            try
                            {
                                ai = atry;
                                const T ans = func_p->initialize(xvalue,a,*vars_p);
                                ai = a0;
                                return ans;
                            }
                            catch(...)
                            {
                                ai = a0; throw;
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
                //______________________________________________________________
                //
                // types
                //______________________________________________________________
                typedef typename Type<T>::Function   Function;   //!< alias
                typedef typename Type<T>::Sequential Sequential; //!< alias
                typedef typename Type<T>::Array      Array;      //!< alias
                typedef typename Type<T>::Matrix     Matrix;     //!< alias
                typedef typename Type<T>::Gradient   Gradient;   //!< alias

                //______________________________________________________________
                //
                // virtual interface
                //______________________________________________________________
                //! destructor
                inline virtual ~SampleType() throw() {}

                //! compute D2 only, for a prepared sample
                /**
                 Yf must be computed along
                 */
                virtual T computeD2(typename Type<T>::Sequential &F,
                                    const Array                  &aorg) = 0;


                //! compute D2, beta and alpha, beta and alpha initialy empty, for a prepared sample
                virtual T computeD2(Sequential        &F,
                                    const Array       &aorg,
                                    Array             &beta,
                                    Matrix            &alpha,
                                    Gradient          &grad,
                                    const array<bool> &used) = 0;

                //! add Sum of Squared Errors (SSE) and Sum of Square Residuals (SSR) and Degrees of Freedom
                virtual void add_SSE_SSR( T &SSE, T &SSR ) const = 0;



                //! compute correlation
                virtual T compute_correlation( correlation<T> &corr ) const = 0;

                //______________________________________________________________
                //
                // non-virtual interface
                //______________________________________________________________
                //! compute D2 only, wrapper for function
                inline T computeD2_(Function     &F,
                                    const Array  &aorg)
                {
                    typename Type<T>::SequentialFunction SF(F);
                    return computeD2(SF,aorg);
                }

                //! compute R2 after a D2 is computed
                inline T computeR2() const
                {
                    T SSE=0, SSR=0;
                    add_SSE_SSR(SSE,SSR);
                    return SSR/(SSR+SSE+numeric<T>::tiny);
                }

            protected:
                //! initialize
                inline explicit SampleType(const size_t nvar_max) :
                SampleInfo(nvar_max),
                rc(nvar_max,as_capacity)
                {
                }

                vector<T>    rc; //!< resources


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
                typedef arc_ptr<Sample>              Pointer;    //!< smart pointer
                typedef vector<Pointer>              Collection; //!< for multiple samples
                typedef typename Type<T>::Function   Function;   //!< alias
                typedef typename Type<T>::Sequential Sequential; //!< alias
                typedef typename Type<T>::Array      Array;      //!< alias
                typedef typename Type<T>::Matrix     Matrix;     //!< alias
                typedef typename Type<T>::Gradient   Gradient;   //!< alias
                typedef SampleInfo::Indices          Indices;    //!< alias


                const Array &X;  //!< X values
                const Array &Y;  //!< Y values
                Array       &Yf; //!< Yf = F(x,...) values
                Indices      J;  //!< ranks of X in decreasing order to call j=J[count()]...J[1]

                //! destructor
                inline virtual ~Sample() throw() {}

                //! initialize
                inline explicit Sample(const Array &userX,
                                       const Array &userY,
                                       Array       &userYf,
                                       const size_t nvar_max=0) :
                SampleType<T>(nvar_max),
                X(userX),
                Y(userY),
                Yf(userYf),
                J(nvar_max,as_capacity)
                {}

                //! X.size()
                virtual inline size_t count() const throw()
                {
                    assert(X.size()==Y.size());
                    assert(X.size()==Yf.size());
                    return X.size();
                }

                //! prepare resources for fit sessions
                virtual void prepare()
                {
                    const size_t n = this->count();
                    this->rc.free();
                    this->rc.ensure(n);
                    J.make(n,0);
                    indexing::make(J, comparison::decreasing<T>, X);
                }

                //! implementation for one sample
                virtual inline void add_SSE_SSR( T &SSE, T &SSR ) const
                {
                    assert(X.size()==Y.size());
                    assert(X.size()==Yf.size());
                    assert(X.size()>0);

                    const size_t n = X.size();

                    // first passs
                    T ybar=0;
                    T sse =0;
                    for(size_t i=n;i>0;--i)
                    {
                        const T Yi = Y[i];
                        ybar += Yi;
                        sse  += square_of(Yf[i]-Yi);
                    }

                    // second pass
                    ybar/=n;
                    T ssr=0;
                    for(size_t i=n;i>0;--i)
                    {
                        ssr += square_of(Yf[i]-ybar);
                    }

                    SSE += sse;
                    SSR += ssr;
                }



                //! compute D2 only from a subsequent call
                virtual T computeD2(Sequential  &F,
                                    const Array &aorg)
                {
                    assert(X.size()==Y.size());
                    assert(X.size()==Yf.size());
                    assert(J.size()==X.size());
                    assert(this->rc.capacity()>=X.size());
                    const size_t n = X.size();
                    if(n>0)
                    {
                        this->rc.free();
                        size_t i=n;
                        // initialize
                        {
                            const size_t j = J[i];
                            const T      Fj = (Yf[j]=F.initialize(X[j],aorg,this->variables));
                            this->rc.push_back_( square_of(Y[j]-Fj) );
                        }
                        // subsequent
                        for(--i;i>0;--i)
                        {
                            const size_t j=J[i];
                            assert(X[j]>=F.current);
                            const T Fj = (Yf[j]=F.compute_to(X[j],aorg,this->variables));
                            this->rc.push_back_( square_of(Y[j]-Fj) );
                        }
                        // sum
                        return sorted_sum(this->rc);
                    }
                    else
                    {
                        return 0;
                    }
                }



                //! compute D2 and sum differential values
                virtual T computeD2(Sequential        &F,
                                    const Array       &aorg,
                                    Array             &beta,
                                    Matrix            &alpha,
                                    Gradient          &grad,
                                    const array<bool> &used)
                {
                    const size_t nvar = aorg.size();
                    assert(beta.size()==nvar);
                    assert(alpha.rows ==nvar);
                    assert(alpha.cols ==nvar);
                    assert(X.size()==Y.size());
                    assert(X.size()==Yf.size());
                    assert(this->rc.capacity()>=X.size());

                    //! compute D2 and fill in Yf[i]
                    const T ans = computeD2(F,aorg);

                    Array &dFda = alpha.r_aux1;
                    for(size_t i=X.size();i>0;--i)
                    {
                        const T Xi  = X[i];
                        const T dFi = Y[i]-Yf[i]; //!< recompute difference
                        grad(dFda,F,Xi,aorg,this->variables,used);
                        for(size_t j=nvar;j>0;--j)
                        {
                            if(used[j])
                            {
                                const T dFda_j = dFda[j];
                                beta[j] += dFi * dFda_j;
                                Array &alpha_j = alpha[j];
                                for(size_t k=j;k>0;--k)
                                {
                                    if(used[k])
                                    {
                                        alpha_j[k] += dFda_j * dFda[k];
                                    }
                                }
                            }
                        }
                    }
                    return ans;
                }

                //! correlation from fitted and source data
                virtual T compute_correlation( correlation<T> &corr ) const
                {
                    corr.free();
                    for(size_t i=X.size();i>0;--i)
                    {
                        corr.add( Y[i], Yf[i] );
                    }
                    return corr.compute();
                }

                //! compute the apparent slope of data in case of linear case
                inline T slope() const throw()
                {
                    T sum_x2 = 0;
                    T sum_xy = 0;
                    for(size_t i=X.size();i>0;--i)
                    {
                        const T x = X[i];
                        sum_x2 += square_of(x);
                        sum_xy += Y[i] * x;
                    }
                    if(sum_x2<=0)
                    {
                        return 0;
                    }
                    else
                    {
                        return sum_xy/sum_x2;
                    }

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

