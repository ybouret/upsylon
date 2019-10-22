
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
                Y_DECL_ARGS(T,type);
                typedef arc_ptr<Sample>               Pointer;
                typedef vector<Sample::Pointer>       Handles;
                typedef typename Type<type>::Sequence Sequence;
                typedef typename Type<type>::Series   Series;
                typedef typename Type<type>::Array    Array;
                typedef typename Type<type>::Matrix   Matrix;

                inline explicit Sample(const Series &userAbscissa,
                                       const Series &userOrdinate,
                                       Series       &userAdjusted) throw() :
                abscissa(userAbscissa),
                ordinate(userOrdinate),
                adjusted(userAdjusted),
                indices(),
                deltaSq()
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
                    indexing::make(indices,comparison::increasing<type>,*abscissa);
                    std::cerr << "abscissa = " << abscissa << std::endl;
                    std::cerr << "indices  = " << indices  << std::endl;
                }

                //! compute D2 using indexed access
                virtual T  compute(Sequential<T>   &F,
                                   const Array     &aorg) const
                {
                    assert(indices.size() == count() );
                    assert(deltaSq.size() == count() );
                    const size_t n = count();
                    if(n>0)
                    {
                        const accessible<type> &X = *abscissa;
                        const accessible<type> &Y = *ordinate;
                        addressable<type>      &Z = *adjusted;

                        // initialize
                        {
                            const size_t i1 = indices[1];
                            const_type   F1 = ( Z[i1] = F.initialize(X[i1],aorg,this->variables) );
                            deltaSq[1]      = square_of( F1-Y[i1] );
                        }

                        // subsequent
                        for(size_t i=2;i<=n;++i)
                        {
                            const size_t  j = indices[i];
                            const type   Fj = ( Z[j] = F.compute_to(X[j],aorg,this->variables) );
                            deltaSq[i]      = square_of(Fj-Y[j]);
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


                        const size_t            nvar = aorg.size();
                        const accessible<type> &X    = *abscissa;
                        const accessible<type> &Y    = *ordinate;
                        addressable<type>      &Z    = *adjusted;
                        addressable<type>      &dY   = deltaSq;
                        dFda.adjust(nvar,0);

                        //------------------------------------------------------
                        // first pass compute fit/store delta Y
                        //------------------------------------------------------

                        // initialize
                        {
                            const size_t i1 = indices[1];
                            const_type   x1 = X[i1];
                            const_type   F1 = ( Z[i1] = F.initialize(x1,aorg,this->variables) );
                            dY[1]           = Y[i1]-F1;
                        }

                        // subsequent
                        for(size_t i=2;i<=n;++i)
                        {
                            const size_t j  = indices[i];
                            const_type   xj = X[j];
                            const_type   Fj = ( Z[j] = F.compute_to(xj,aorg,this->variables) );
                            dY[i]           = Y[j] - Fj;
                        }

                        //------------------------------------------------------
                        // second pass : costly gradient
                        //------------------------------------------------------
                        for(size_t ii=n;ii>0;--ii)
                        {
                            const size_t i   = indices[ii];
                            const_type   X_i = X[i];
                            grad(dFda,F,X_i,aorg,this->variables,used);
                            const_type   dY_i = dY[i];
                            for(size_t j=nvar;j>0;--j)
                            {
                                // update beta[j]
                                const_type dFda_j = dFda[j];
                                beta[j] += dY_i * dFda_j;

                                // update alpha[j][k<=j]
                                Array &alpha_j = alpha[j];
                                for(size_t k=j;k>0;--k)
                                {
                                    alpha_j[k] += dFda_j * dFda[k];
                                }
                            }

                        }


                        //------------------------------------------------------
                        // third pass: build squares for delta
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
                Indices                      indices;
                mutable vector<mutable_type> deltaSq;
                mutable vector<mutable_type> dFda;

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

