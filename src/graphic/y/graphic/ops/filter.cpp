
#include "y/graphic/ops/filter.hpp"
#include "y/sort/heap.hpp"
#include "y/comparison.hpp"

namespace upsylon {

    namespace Graphic {
        
        namespace Kernel {

            Filter:: ~Filter() throw()
            {
            }

            Filter:: Filter( const size_t n) : Object(), weights(n)
            {
            }

            Filter:: Filter( const Filter &F ) : Object(), weights(F.weights)
            {
            }
            

            static inline
            int CompareWeights( const Filter::Weight &lhs, const Filter::Weight &rhs) throw()
            {
                return comparison::decreasing_lexicographic(&lhs.point.x, &rhs.point.x, 2);
            }

            void Filter:: update() throw()
            {
                lightweight_array<Weight> arr( &weights[0], weights.size() );
                hsort(arr,CompareWeights);
            }

        }

    }
}
