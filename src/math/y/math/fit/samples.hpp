//! \file
#ifndef Y_MATH_FIT_SAMPLES_INCLUDED
#define Y_MATH_FIT_SAMPLES_INCLUDED 1

#include "y/math/fit/types.hpp"
#include "y/math/stat/metrics.hpp"
#include "y/sort/sorted-sum.hpp"
#include "y/sort/index.hpp"
#include "y/ios/ostream.hpp"

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

                //! save to logfile
                inline
                void writeLog(ios::ostream     &fp,
                              const Array       &aorg,
                              const Array       &aerr,
                              const array<bool> &used,
                              const char         com = '#') const
                {
                    long   dof         = this->count();
                    size_t name_maxlen = 0;
                    size_t aorg_maxlen = 0;
                    size_t aerr_maxlen = 0;
                    size_t dim         = 0;
                    vector<string>    aVal( variables.size(), as_capacity );
                    vector<string>    eVal( variables.size(), as_capacity );

                    // first pass
                    for( Variables::const_iterator i=variables.begin();i!=variables.end();++i)
                    {
                        const Variable &v = **i;
                        name_maxlen = max_of(name_maxlen,v.name.size());
                        if( used[v.check_index(used.size()) ] )
                        {
                            --dof;
                            ++dim;
                        }

                        {
                            const string value = vformat("%.15g", aorg[ v.check_index( aorg.size() ) ] );
                            aVal.push_back(value);
                            aorg_maxlen = max_of( aorg_maxlen, value.size() );
                        }

                        {
                            const string value = vformat("%.15g", aerr[ v.check_index( aerr.size() ) ] );
                            eVal.push_back(value);
                            aerr_maxlen = max_of( aerr_maxlen, value.size() );
                        }

                    }
                    // header
                    fp << com << " #data      = "; fp("%u",unsigned(this->count()))    << '\n';
                    fp << com << " #variables = "; fp("%u",unsigned(variables.size())) << '\n';
                    fp << com << " #dimension = "; fp("%u",unsigned(dim))              << '\n';
                    fp << com << " #dof       = "; fp("%ld",dof)                       << '\n';
                    correlation<T> corr;
                    fp << com << "       corr = "; fp("%.15g",this->compute_correlation(corr)) << '\n';
                    fp << com << "         R2 = "; fp("%.15g",this->computeR2())               << '\n';
                    
                    // second pass save variables
                    size_t iv = 0;
                    for( Variables::const_iterator i=variables.begin();i!=variables.end();++i )
                    {
                        ++iv;
                        const Variable &v    = **i;
                        const string    name = v.name;
                        const string   &val  = aVal[iv];
                        const string   &err  = eVal[iv];
                        const bool      use  = used[ v.check_index( used.size() ) ];
                        fp << name;          for(size_t j=name.size();j<=name_maxlen;++j) fp << ' ';
                        fp << "= " << val;    for(size_t j=val.size(); j<=aorg_maxlen;++j) fp << ' ';
                        fp << "+/- " << err; for(size_t j=err.size(); j<=aerr_maxlen;++j) fp << ' ';
                        if(use)
                        {
                            const T re = variables.compute_relative_error(aorg[ v.check_index( aorg.size() ) ],aerr[ v.check_index( aerr.size() ) ]);
                            fp("(%6.2f%%)",re);
                        }
                        else
                        {
                            fp << "( fixed )";
                        }
                        fp << '\n';
                    }



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

                //! get min coordinate
                inline const T lower() const throw()
                {
                    const size_t n = this->count();
                    if(n>0)
                    {
                        assert( n == J.size() );
                        return X[ J[n] ];
                    }
                    else
                    {
                        return 0;
                    }
                }

                //! get max coordinate
                inline const T upper() const throw()
                {
                    const size_t n = this->count();
                    if(n>0)
                    {
                        assert( n == J.size() );
                        return X[ J[1] ];
                    }
                    else
                    {
                        return 0;
                    }
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

