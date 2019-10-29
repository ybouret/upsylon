
//! \file

#ifndef Y_MATH_ADJUST_SAMPLE_INCLUDED
#define Y_MATH_ADJUST_SAMPLE_INCLUDED 1

#include "y/math/adjust/sample/type.hpp"
#include "y/sort/index.hpp"
#include "y/sort/sorted-sum.hpp"
#include "y/ios/ostream.hpp"
#include "y/math/stat/metrics.hpp"

namespace upsylon {

    namespace math {

        namespace Adjust {


            template <typename T> class Samples; //!< forward declaration

            //==================================================================
            //
            //
            //! a single sample
            //
            //
            //==================================================================
            template <typename T>
            class Sample : public SampleType<T>, public counted_object
            {
            public:
                //==============================================================
                //
                // types and definitions
                //
                //==============================================================
                typedef arc_ptr<Sample>            Pointer;  //!< alias for Samples
                typedef vector<Sample::Pointer>    Handles;  //!< alias for Samples
                typedef typename Type<T>::Sequence Sequence; //!< alias
                typedef typename Type<T>::Series   Series;   //!< alias
                typedef typename Type<T>::Matrix   Matrix;   //!< alias

                //==============================================================
                //
                // virtual interface
                //
                //==============================================================
                //! cleanup
                inline virtual ~Sample() throw() {}



                //! common series size
                inline virtual size_t count() const throw()
                {
                    assert(abscissa->size()==ordinate->size());
                    assert(adjusted->size()==abscissa->size());
                    return abscissa->size();
                }

                //! adjust memory and compute indices
                inline virtual void ready()
                {
                    // memory
                    const size_t n = this->count();
                    indices.adjust(n,0);
                    deltaSq.adjust(n,0);

                    // indexing
                    indexing::make(indices,comparison::increasing<T>,*abscissa);
                }

                //! compute D2 using indexed access
                inline virtual T  compute(Sequential<T>       &F,
                                          const accessible<T> &aorg) const
                {
                    assert(indices.size() == count() );
                    assert(deltaSq.size() == count() );
                    const size_t n = count();
                    if(n>0)
                    {
                        const accessible<T> &X = *abscissa;
                        const accessible<T> &Y = *ordinate;
                        addressable<T>      &Z = *adjusted;
                        //------------------------------------------------------
                        // initialize
                        //------------------------------------------------------
                        {
                            const size_t i1 = indices[1];
                            const T      F1 = ( Z[i1] = F.start(X[i1],aorg,this->variables) );
                            deltaSq[1]      = square_of( Y[i1] - F1 );
                        }

                        //------------------------------------------------------
                        // subsequent
                        //------------------------------------------------------
                        for(size_t i=2;i<=n;++i)
                        {
                            const size_t  j  = indices[i];
                            const T       Fj = ( Z[j] = F.reach(X[j],aorg,this->variables) );
                            deltaSq[i]       = square_of(Y[j]-Fj);
                        }

                        return sorted_sum(deltaSq)/2;
                    }
                    else
                    {
                        return 0;
                    }
                }

                //! in-place square value
                static inline void to_square( T &value ) throw()
                {
                    value *= value;
                }

                //! SampleType interface
                virtual T  computeAndUpdate(Matrix                 &alpha,
                                            addressable<T>         &beta,
                                            Sequential<T>          &F,
                                            const accessible<T>    &aorg,
                                            const accessible<bool> &used,
                                            Gradient<T>            &grad) const
                {
                    assert(indices.size() == count() );
                    assert(deltaSq.size() == count() );
                    const size_t n = count();
                    if(n>0)
                    {
                        assert(aorg.size()==used.size());
                        assert(aorg.size()==beta.size());
                        assert(aorg.size()==alpha.rows);
                        assert(aorg.size()==alpha.cols);


                        const size_t        nvar = aorg.size();
                        const accessible<T> &X   = *abscissa;
                        const accessible<T> &Y   = *ordinate;
                        addressable<T>      &Z   = *adjusted;
                        addressable<T>      &dY  = deltaSq;
                        dFda.adjust(nvar,0);

                        //------------------------------------------------------
                        // first pass compute fit/store delta Y
                        //------------------------------------------------------

                        // initialize
                        {
                            const size_t i1 = indices[1];
                            const T      x1 = X[i1];
                            const T      F1 = ( Z[i1] = F.start(x1,aorg,this->variables) );
                            dY[i1]          = Y[i1]-F1;
                        }

                        // subsequent
                        for(size_t ii=2;ii<=n;++ii)
                        {
                            const size_t i   = indices[ii];
                            const T      X_i = X[i];
                            const T      F_i = ( Z[i] = F.reach(X_i,aorg,this->variables) );
                            dY[i]            = Y[i] - F_i;
                        }

                        //------------------------------------------------------
                        // second pass : costly gradient
                        //------------------------------------------------------
                        for(size_t ii=n;ii>0;--ii)
                        {
                            const size_t i   = indices[ii];
                            const T      X_i = X[i];
                            grad(dFda,F,X_i,aorg,this->variables,used);
                            const T     dY_i = dY[i];
                            for(size_t j=nvar;j>0;--j)
                            {
                                if(used[j])
                                {
                                    // update beta[j]
                                    const T dFda_j = dFda[j];
                                    beta[j] += dY_i * dFda_j;

                                    // update alpha[j][k<=j]
                                    addressable<T>  &alpha_j = alpha[j];
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


                        //------------------------------------------------------
                        // third pass: build squares for delta and return D2
                        //------------------------------------------------------
                        for(size_t j=n;j>0;--j)
                        {
                            to_square(dY[j]);
                        }
                        return sorted_sum(dY)/2;
                    }
                    else
                    {
                        return 0;
                    }
                }


                //! activate all matching variables
                inline virtual void activate( addressable<bool> &target, const accessible<bool> &source ) const
                {
                    this->variables.activate(target,source);
                 }




                //==============================================================
                //
                // non-virtual interface
                //
                //==============================================================

                //! setup with Series
                inline explicit Sample(const Series &userAbscissa,
                                       const Series &userOrdinate,
                                       Series       &userAdjusted) throw() :
                abscissa(userAbscissa),
                ordinate(userOrdinate),
                adjusted(userAdjusted),
                indices(),
                deltaSq(),
                dFda()
                {}

                //! output helper
                inline void save( ios::ostream &fp, const bool indexed = false ) const
                {
                    const size_t n = count();
                    if(indexed)
                    {
                        for(size_t i=1;i<=n;++i)
                        {
                            save_triplet(fp,indices[i]);
                        }

                    }
                    else
                    {
                        for(size_t i=1;i<=n;++i)
                        {
                            save_triplet(fp,i);
                        }
                    }
                }

                //! local correlation
                T computeCorrelation( correlation<T> &corr ) const
                {
                    const size_t np = count();
                    corr.free();
                    corr.ensure(np);
                    
                    for(size_t i=np;i>0;--i)
                    {
                        corr.add( ordinate[i], adjusted[i] );
                    }

                    return corr.compute();
                }


                virtual void addToSumOfSquares( T &total, T &residual ) const throw()
                {
                    const size_t np = count();
                    if(np>0)
                    {
                        addressable<T> &temp = deltaSq;

                        for(size_t i=np;i>0;--i)
                        {
                            temp[i] = square_of( ordinate[i] - adjusted[i]  );
                        }
                        residual += sorted_sum(temp);

                        for(size_t i=np;i>0;--i)
                        {
                            temp[i]= ordinate[i];
                        }
                        const T ave = sorted_sum_by_abs(temp)/np;
                        for(size_t i=np;i>0;--i)
                        {
                            temp[i] = square_of(ordinate[i]-ave);
                        }
                        total += sorted_sum(temp);
                    }
                }

                //==============================================================
                //
                // members
                //
                //==============================================================
                const Series   abscissa; //!< abscissa series
                const Series   ordinate; //!< ordinate series
                mutable Series adjusted; //!< adjusted series

            private:
                Indices           indices;
                mutable vector<T> deltaSq;
                mutable vector<T> dFda;

                Y_DISABLE_COPY_AND_ASSIGN(Sample);
                inline void save_triplet( ios::ostream & fp, const size_t i) const
                {
                    fp("%.15g %.15g %.15f\n", double(abscissa[i]),double(ordinate[i]),double(adjusted[i]));
                }

                friend class Samples<T>;

                virtual void collect(sequence<void*> &seq) const
                {
                    const Sample *self = this;
                    seq.push_back( (Sample *)self  );
                }

                
            };

        }
        
    }

}


#endif

