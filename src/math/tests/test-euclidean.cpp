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


        
        static inline void make(const char *TID, const char *PID)
        {

            
            std::cerr << "Using " <<PID << "<" << TID << ">" << std::endl;
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
                }
                Y_CHECK(np==sa.nodes.size());
                Y_CHECK(np==pa.nodes.size());

                
                std::cerr << "standard:" << sa.nodes.size() << "/" << sa.segments.size() << std::endl;
                std::cerr << "periodic:" << pa.nodes.size() << "/" << pa.segments.size() << std::endl;


                const string s_pfx = "std_";
                const string p_pfx = "per_";

                {
                    const string  sfn = s_pfx + PID + '_' + TID + ".dat";
                    const string  pfn = p_pfx + PID + '_' + TID + ".dat";
                    ios::ocstream sfp(sfn);
                    ios::ocstream pfp(pfn);
                    for(size_t i=1;i<=np;++i)
                    {
                        PointType::Print(sfp, sa.nodes[i]->point->position ) << '\n';
                        PointType::Print(pfp, pa.nodes[i]->point->position ) << '\n';
                    }
                    PointType::Print(pfp, pa.nodes[1]->point->position );
                }

                sa.celerities();
                pa.celerities();
                {
                    const string  sfn = s_pfx + PID + '_' + TID + "_v.dat";
                    const string  pfn = p_pfx + PID + '_' + TID + "_v.dat";
                    ios::ocstream sfp(sfn);
                    ios::ocstream pfp(pfn);
                    for(size_t i=1;i<=np;++i)
                    {
                        {
                            Vertex p = sa.nodes[i]->point->position;
                            PointType::Print(sfp, p ) << '\n';
                            p += sa.nodes[i]->celerity/2;
                            PointType::Print(sfp, p ) << '\n' << '\n';

                        }
                        
                        {
                            Vertex p = pa.nodes[i]->point->position;
                            PointType::Print(pfp, p ) << '\n';
                            p += pa.nodes[i]->celerity/2;
                            PointType::Print(pfp, p ) << '\n' << '\n';

                        }
                    }
                }

                sa.compile();
                pa.compile();

                {
                    const string  sfn = s_pfx + PID + '_' + TID + "_i.dat";
                    ios::ocstream sfp(sfn);
                    for(mutable_type x=0; x <= np-1; x += mutable_type(0.01) )
                    {
                       // const Vertex v = sa(x);
                       // PointType::Print(sfp,v) << '\n';
                    }
                }

            }

        }
    };
    
}

#define _EUCLIDEAN(TYPE,POINT) euclidean_test<TYPE,POINT>::make(#TYPE,#POINT)

Y_UTEST(euclidean)
{

    _EUCLIDEAN(float,point2d);
    _EUCLIDEAN(double,point2d);

    _EUCLIDEAN(float,point3d);
    _EUCLIDEAN(double,point3d);

    _EUCLIDEAN(float,complex);
    _EUCLIDEAN(double,complex);
    
}
Y_UTEST_DONE()



