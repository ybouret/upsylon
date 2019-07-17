//! \file
#ifndef Y_MATH_FIT_SAMPLE_INCLUDED
#define Y_MATH_FIT_SAMPLE_INCLUDED 1

#include "y/math/fit/sample-type.hpp"
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


        }

    }

}

#endif
