
//! \file

#ifndef Y_MATH_ADJUST_SAMPLE_INCLUDED
#define Y_MATH_ADJUST_SAMPLE_INCLUDED 1

#include "y/math/adjust/sample/type.hpp"
#include "y/sort/index.hpp"
#include "y/sort/sorted-sum.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon {

    namespace math {

        namespace Adjust {

            template <typename T>
            class Sample : public SampleType<T>, public counted_object
            {
            public:
                typedef arc_ptr<Sample>            Pointer;
                typedef vector<Sample::Pointer>    Handles;
                typedef typename Type<T>::Sequence Sequence;
                typedef typename Type<T>::Series   Series;
                typedef typename Type<T>::Array    Array;
                typedef typename Type<T>::Matrix   Matrix;

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

                inline virtual ~Sample() throw() {}

                const Series   abscissa;
                const Series   ordinate;
                mutable Series adjusted;

                inline virtual size_t count() const throw()
                {
                    assert(abscissa->size()==ordinate->size());
                    assert(adjusted->size()==abscissa->size());
                    return abscissa->size();
                }

                inline virtual void ready()
                {
                    // memory
                    const size_t n = this->count();
                    indices.adjust(n,0);
                    deltaSq.adjust(n,0);

                    // indexing
                    indexing::make(indices,comparison::increasing<T>,*abscissa);
                    std::cerr << "abscissa = " << abscissa << std::endl;
                    std::cerr << "indices  = " << indices  << std::endl;
                }

                //! compute D2 using indexed access
                inline virtual T  compute(Sequential<T>   &F,
                                          const Array     &aorg) const
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
                            const T      F1 = ( Z[i1] = F.initialize(X[i1],aorg,this->variables) );
                            deltaSq[1]      = square_of( Y[i1] - F1 );
                        }

                        //------------------------------------------------------
                        // subsequent
                        //------------------------------------------------------
                        for(size_t i=2;i<=n;++i)
                        {
                            const size_t  j  = indices[i];
                            const T       Fj = ( Z[j] = F.compute_to(X[j],aorg,this->variables) );
                            deltaSq[i]       = square_of(Y[j]-Fj);
                        }

                        return sorted_sum(deltaSq)/2;
                    }
                    else
                    {
                        return 0;
                    }
                }


                static inline void to_square( T &value ) throw()
                {
                    value *= value;
                }

                virtual T  computeAndUpdate(Matrix          &alpha,
                                            Array           &beta,
                                            Sequential<T>   &F,
                                            const Array     &aorg,
                                            const Flags     &used,
                                            Gradient<T>     &grad) const
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
                        const accessible<T> &X    = *abscissa;
                        const accessible<T> &Y    = *ordinate;
                        addressable<T>      &Z    = *adjusted;
                        addressable<T>      &dY   = deltaSq;
                        dFda.adjust(nvar,0);

                        //------------------------------------------------------
                        // first pass compute fit/store delta Y
                        //------------------------------------------------------

                        // initialize
                        {
                            const size_t i1 = indices[1];
                            const T      x1 = X[i1];
                            const T      F1 = ( Z[i1] = F.initialize(x1,aorg,this->variables) );
                            dY[i1]          = Y[i1]-F1;
                        }

                        // subsequent
                        for(size_t ii=2;ii<=n;++ii)
                        {
                            const size_t i   = indices[ii];
                            const T      X_i = X[i];
                            const T      F_i = ( Z[i] = F.compute_to(X_i,aorg,this->variables) );
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


                inline virtual void activate( addressable<bool> &target, const accessible<bool> &source ) const
                {
                    this->variables.activate(target,source);
                 }


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

            private:
                Indices           indices;
                mutable vector<T> deltaSq;
                mutable vector<T> dFda;

                Y_DISABLE_COPY_AND_ASSIGN(Sample);
                inline void save_triplet( ios::ostream & fp, const size_t i) const
                {
                    fp("%.15g %.15g %.15f\n", double(abscissa[i]),double(ordinate[i]),double(adjusted[i]));
                }

                
            };

        }
        
    }

}


#endif

