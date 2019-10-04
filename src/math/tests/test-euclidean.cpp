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
        Y_EUCLIDEAN_POINT_ARGS();     //!< aliases
        Y_EUCLIDEAN_SEGMENT_ARGS();   //!< aliases
        Y_EUCLIDEAN_POINTNODE_ARGS(); //!< aliases
        
        typedef Segments<T,POINT>           SegmentsType;
        
        static inline void make()
        {
            const std::type_info &tid = typeid(T);
            const std::type_info &pid = typeid(PointType);
            
            std::cerr << "Using " << pid.name() << "/" << tid.name() << std::endl;
            std::cerr << "|_sizeof(type)     = " << sizeof(T)           << std::endl;
            std::cerr << "|_sizeof(point)    = " << sizeof(PointType)   << std::endl;
            std::cerr << "|_sizeof(shared)   = " << sizeof(SharedPoint) << std::endl;
            std::cerr << "|_sizeof(node)     = " << sizeof(NodeType)    << std::endl;
            std::cerr << "|_sizeof(list)     = " << sizeof(NodeList)    << std::endl;
            std::cerr << "|_sizeof(segment)  = " << sizeof(SegmentType) << std::endl;
            
            {
                NodeList points;
                for(size_t i=10+alea.leq(100);i>0;--i)
                {
                    SharedPoint P = new PointType();
                    points.push_back( new NodeType(P) );
                }
                
                SegmentsType segments;
                for(size_t i=10+alea.leq(100);i>0;--i)
                {
                    const SharedPoint &a = *points.fetch( alea.lt(points.size) );
                    const SharedPoint &b = *points.fetch( alea.lt(points.size) );
                    segments(a,b);
                }
                
                segments.update();
            }
            
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
                    Y_ASSERT(sa.check());
                    Y_ASSERT(pa.check());
                }
                
                std::cerr << "standard:" << sa.points.size << "/" << sa.segments.size << std::endl;
                std::cerr << "periodic:" << pa.points.size << "/" << pa.segments.size << std::endl;
                
                {
                    const string sfn = vformat("%s_sa.dat",pid.name());
                    const string pfn = vformat("%s_pa.dat",pid.name());
                    
                    ios::ocstream sfp(sfn);
                    ios::ocstream pfp(pfn);
                    for( const NodeType *snode=sa.points.head,
                        *pnode = pa.points.head;
                        snode;snode=snode->next,pnode=pnode->next)
                    {
                        PointType::Print(sfp,(**snode).position)<< '\n';
                        PointType::Print(pfp,(**pnode).position)<< '\n';
                    }
                    PointType::Print(pfp,(**pa.points.head).position)<< '\n';

                }
                
                sa.celerities();
                pa.celerities();
                
                {
                    const string sfn = vformat("%s_sa_v.dat",pid.name());
                    const string pfn = vformat("%s_pa_v.dat",pid.name());
                    
                    ios::ocstream sfp(sfn);
                    ios::ocstream pfp(pfn);
                    for( const NodeType *snode=sa.points.head,
                        *pnode = pa.points.head;
                        snode;snode=snode->next,pnode=pnode->next)
                    {
                        {
                            Vertex         p = (**snode).position;
                            const Vertex   v = snode->celerity;
                            PointType::Print(sfp,p) << '\n';
                            p+=v/2;
                            PointType::Print(sfp,p) << '\n' << '\n';
                        }
                        
                        {
                            Vertex         p = (**pnode).position;
                            const Vertex   v = pnode->celerity;
                            PointType::Print(pfp,p) << '\n';
                            p+=v/2;
                            PointType::Print(pfp,p) << '\n' << '\n';
                        }

                    }
                }

                pa.segments.update();
                sa.segments.update();
                std::cerr << "length: standard: " << sa.segments.length << std::endl;
                std::cerr << "        periodic: " << pa.segments.length << std::endl;

                {
                    const string sfn = vformat("%s_sa_s.dat",pid.name());
                    const string pfn = vformat("%s_pa_s.dat",pid.name());
                    ios::ocstream sfp(sfn);
                    ios::ocstream pfp(pfn);
                    unsigned i=0;
                    for( const NodeType
                        *snode = sa.points.head,
                        *pnode = pa.points.head;
                        snode;snode=snode->next,pnode=pnode->next,++i)
                    {
                        sfp("%u %.15g\n", i, snode->speed);
                        pfp("%u %.15g\n", i, pnode->speed);
                    }
                    pfp("%u %.15g\n", i, pa.points.head->speed);
                }
            }
            
        }
    };
    
}


Y_UTEST(euclidean)
{
    
    euclidean_test<float,point2d>::make();
    euclidean_test<double,point2d>::make();
    
    euclidean_test<float, point3d>::make();
    euclidean_test<double,point3d>::make();
    
    euclidean_test<float,complex>::make();
    euclidean_test<double,complex>::make();
    
    
}
Y_UTEST_DONE()



