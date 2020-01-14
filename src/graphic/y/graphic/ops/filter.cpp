
#include "y/graphic/ops/filter.hpp"
#include "y/sort/heap.hpp"
#include "y/comparison.hpp"
#include "y/memory/pooled.hpp"
#include "y/sequence/vector.hpp"
#include "y/sort/sorted-sum.hpp"

namespace upsylon {

    namespace Graphic {
        
        namespace Kernel {

            Filter:: Weight:: ~Weight() throw()
            {
                _bzset(value);
                _bzset(point);
            }

            Filter:: Weight:: Weight(const float f,
                                     const unit_t dx,
                                     const unit_t dy)  throw() :
            value(f),
            point(dx,dy)
            {
            }


            Filter:: Weight:: Weight(const Weight &other) throw() :
            value( other.value ),
            point( other.point )
            {

            }


            Filter:: ~Filter() throw()
            {
                _bzset(weight);
                _bzset(factor);
            }

            Filter:: Filter( const size_t n) : Object(), weights(n), weight(0.0f), factor(0)
            {
            }

            Filter:: Filter( const Filter &F ) :
            Object(), weights(F.weights), weight(F.weight), factor(F.factor)
            {
            }
            

            static inline
            int CompareWeights( const Filter::Weight &lhs, const Filter::Weight &rhs) throw()
            {
                return comparison::decreasing_lexicographic(&lhs.point.x, &rhs.point.x, 2);
            }

            void Filter:: update()
            {
                aliasing::_(factor) = 1.0f;
                aliasing::_(weight) = 0.0f;
                const size_t    num = weights.size();
                if(num>0)
                {
                    lightweight_array<Weight> arr(&weights[0],num);
                    hsort(arr,CompareWeights);
                    {
                        vector<float,memory::pooled> W(num,as_capacity);
                        for(size_t i=1;i<=num;++i)
                        {
                            W.push_back_( square_of(arr[i].value) );
                        }
                        aliasing::_(weight) = sqrtf( sorted_sum(W) );
                    }

                    if(fabsf(weight)>0)
                    {
                        aliasing::_(factor) = 1.0f/weight;
                    }
                }
            }

            void Filter:: normalize() throw()
            {
                const size_t num = weights.size();
                if(num>0)
                {
                }
            }


        }

    }
}
