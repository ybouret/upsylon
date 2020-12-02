#include "y/mkl/geometric/arc/compiler.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/associative/hash/set.hpp"
#include "support.hpp"
#include "y/type/spec.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace mkl;
using namespace Geometric;

namespace {

#if 0
    static inline string type2file( const std::type_info &tid)
    {
        string id = type_name_for(tid);
        for(size_t i=0;i<id.size();++i)
        {
            char C = id[i];
            switch(C)
            {
                case '<':
                case '>':
                case ',':
                    C = '_';
                    break;

                default:
                    break;
            }
            id[i] = C;
        }
        return id;
    }
#endif
    
    template <typename T, template <class> class VTX>
    static inline void testPoints()
    {

        typedef Point<T,VTX>                PointType;
        typedef typename PointType::Pointer PointerType;
        typedef typename PointType::vertex  vertex;

        std::cerr << "points: " << type_name_of<vertex>() << std::endl;
        std::cerr << "  dims: " << PointType::Dimensions << std::endl;

        typedef hash_set<PointKey,PointerType> Points;

        Points       points;
        const size_t np = 1+alea.leq(10);
        const vertex center = support::get<vertex>();
        const vertex radius = support::get<vertex>();

        for(size_t i=0;i<np;++i)
        {
            const T theta     = (numeric<T>::two_pi*i)/np;
            const T *r        = (const T *)&radius;
            const T *c        = (const T *)&center;
            const T  coord[3] = { c[0] + r[0] * cos_of(theta), c[1] + r[1] * sin_of(theta), c[2] };
            PointerType p = new PointType();
            memcpy( & **p, coord, sizeof(T) * PointType::Dimensions );
            Y_ASSERT(points.insert(p));
        }
        std::cerr << "points=" <<  points << std::endl;

        typedef Node<T,VTX>                NodeType;
        typedef typename NodeType::Pointer NodePointer;
        hash_set<NodeKey,NodePointer>      nodes;
        for(typename Points::iterator it=points.begin();it!=points.end();++it)
        {
            const NodePointer n = new NodeType( *it );
            Y_ASSERT(nodes.insert(n));
        }
        std::cerr << "nodes=" << nodes << std::endl;

        StandardArc<T,VTX> sa;
        PeriodicArc<T,VTX> pa;
        for(typename Points::iterator it=points.begin();it!=points.end();++it)
        {
            sa.insert(*it);
            pa.insert(*it);
        }

         

        std::cerr << "sizes: " << std::endl;
        Y_UTEST_SIZEOF(vertex);
        Y_UTEST_SIZEOF(PointType);
        Y_UTEST_SIZEOF(NodeType);
        std::cerr << std::endl;
    }

}

Y_UTEST(geometric)
{

    testPoints<float,point2d>();
    testPoints<float,point3d>();
    testPoints<float,complex>();

    testPoints<double,point2d>();
    testPoints<double,point3d>();
    testPoints<double,complex>();

}
Y_UTEST_DONE()

