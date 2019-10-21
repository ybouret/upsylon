
//! \file

#ifndef Y_MATH_ADJUST_SAMPLE_INCLUDED
#define Y_MATH_ADJUST_SAMPLE_INCLUDED 1

#include "y/math/adjust/sample/type.hpp"
#include "y/sort/index.hpp"
#include "y/sort/sorted-sum.hpp"

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

                virtual T  computeD2(Sequential<T>   &F,
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

                        return sorted_sum(deltaSq);
                    }
                    else
                    {
                        return 0;
                    }
                }

            private:
                Indices                      indices;
                mutable vector<mutable_type> deltaSq;

                Y_DISABLE_COPY_AND_ASSIGN(Sample);
            };

        }
        
    }

}


#endif

