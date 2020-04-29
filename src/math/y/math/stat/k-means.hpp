//! \file

#ifndef Y_MATH_K_MEANS_INCLUDED
#define Y_MATH_K_MEANS_INCLUDED 1

#include "y/type/point3d.hpp"
#include "y/core/inode.hpp"

namespace upsylon {
    
    namespace math {
        
        namespace kernel
        {
            template <size_t DIM> struct real_vertex;
            template <>           struct real_vertex<1> { typedef double          type; };
            template <>           struct real_vertex<2> { typedef point2d<double> type; };
            template <>           struct real_vertex<3> { typedef point3d<double> type; };
        }
        
        template <typename T,typename VERTEX>
        struct k_means
        {
            static  const size_t  dimensions = sizeof(VERTEX)/sizeof(T);
            typedef typename kernel::real_vertex<dimensions>::type vertex;
            
            class vnode : public inode<vnode>
            {
            public:
                const VERTEX &v; //!< vertex reference
                const size_t  n; //!< number of vertex=weight
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(vnode);
            };
        };
        
    }
}


#endif


