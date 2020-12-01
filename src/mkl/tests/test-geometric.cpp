#include "y/mkl/geometric/arc/standard.hpp"
#include "y/mkl/geometric/arc/periodic.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/associative/hash/set.hpp"
#include "support.hpp"
#include "y/type/spec.hpp"

using namespace upsylon;
using namespace mkl;
using namespace Geometric;

namespace {

    template <typename T, template <class> class VTX>
    static inline void testPoints()
    {

        typedef Point<T,VTX>                PointType;
        typedef typename PointType::Pointer PointerType;
        typedef typename PointType::vertex  vertex;

        std::cerr << "points: " << type_name_of<vertex>() << std::endl;
        std::cerr << "  dims: " << PointType::Dimensions << std::endl;

        typedef hash_set<PointKey,PointerType> Points;
        Points points;

        for(size_t i=3+alea.leq(7);i>0;--i)
        {
            const PointerType p = new PointType( support::get<vertex>() );
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

        std::cerr << "sa: nodes=" << sa.nodes.size() << " segments=" << sa.segments.size << std::endl;
        std::cerr << "pa: nodes=" << pa.nodes.size() << " segments=" << pa.segments.size << std::endl;

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

