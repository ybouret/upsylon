#include "y/math/euclidean/standard-arc.hpp"
#include "y/math/euclidean/periodic-arc.hpp"

#include "y/type/complex.hpp"
#include "y/utest/run.hpp"
#include <typeinfo>

#include "y/ios/ocstream.hpp"
#include "y/string.hpp"

using namespace upsylon;
using namespace math;
using namespace Euclidean;

namespace {
    
    template <typename T, template <class> class POINT>
    struct euclidean_test
    {
        Y_EUCLIDEAN_ARC_ARGS();


        
        static inline void make()
        {
            const std::type_info &tid = typeid(T);
            const std::type_info &pid = typeid(PointType);
            
            std::cerr << "Using " << pid.name() << "/" << tid.name() << std::endl;
            std::cerr << "|_sizeof(type)     = " << sizeof(T)           << std::endl;
            std::cerr << "|_sizeof(point)    = " << sizeof(PointType)   << std::endl;
            std::cerr << "|_sizeof(shared)   = " << sizeof(SharedPoint) << std::endl;
            std::cerr << "|_sizeof(node)     = " << sizeof(NodeType)    << std::endl;
            std::cerr << "|_sizeof(segment)  = " << sizeof(SegmentType) << std::endl;

            
            {
                std::cerr << "Creating Arcs..." << std::endl;
                StandardArc<T,POINT> sa;
                PeriodicArc<T,POINT> pa;
                const size_t np = 10+alea.leq(10);
                for(size_t i=0;i<np;++i)
                {
                    const float  theta = (numeric<float>::two_pi * i)/np;
                    const float  x     = cosf(theta) + 0.1f * alea.symm<float>();
                    const float  y     = sinf(theta) + 0.1f * alea.symm<float>();
                    const float  z     = float(i)/np;
                    const type   arr[4] = { x,y,z, 0 };
                    const Vertex     &v  = *(const Vertex *) &arr[0];
                    const SharedPoint sp = new PointType(v);
                    sa << sp;
                    pa << sp;
                    //Y_ASSERT(sa.check());
                    //Y_ASSERT(pa.check());
                }
                
                std::cerr << "standard:" << sa.nodes.size() << "/" << sa.segments.size() << std::endl;
                std::cerr << "periodic:" << pa.nodes.size() << "/" << pa.segments.size() << std::endl;

                sa.celerities();

            }

        }
    };
    
}


Y_UTEST(euclidean)
{

#if 1
    euclidean_test<float,point2d>::make();
    euclidean_test<double,point2d>::make();
    
    euclidean_test<float, point3d>::make();
    euclidean_test<double,point3d>::make();
    
    euclidean_test<float,complex>::make();
    euclidean_test<double,complex>::make();
#endif
    
}
Y_UTEST_DONE()



