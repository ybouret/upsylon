#include "y/math/euclidean/arc/standard.hpp"
#include "y/math/euclidean/arc/periodic.hpp"
#include "y/math/fcn/integrate.hpp"

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


        
        static inline void make(const char *TID, const char *PID, const size_t nn)
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
                const size_t np = (nn>0) ? nn : 10+alea.leq(10);
                for(size_t i=0;i<np;++i)
                {
                    const float  theta = (numeric<float>::two_pi * i)/np;
                    const float  x     = cosf(theta)  + 0.1f * alea.symm<float>();
                    const float  y     = sinf(theta)  + 0.1f * alea.symm<float>();
                    const float  z     = sinf(theta/2) + 0.1f * alea.symm<float>();
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

                sa.metrics();
                pa.metrics();
                {
                    const string  sfn = s_pfx + PID + '_' + TID + "_t.dat";
                    const string  pfn = p_pfx + PID + '_' + TID + "_t.dat";
                    ios::ocstream sfp(sfn);
                    ios::ocstream pfp(pfn);
                    for(size_t i=1;i<=np;++i)
                    {
                        {
                            Vertex p = sa.nodes[i]->point->position;
                            PointType::Print(sfp, p ) << '\n';
                            p += sa.nodes[i]->V/2;
                            PointType::Print(sfp, p ) << '\n' << '\n';

                        }
                        
                        {
                            Vertex p = pa.nodes[i]->point->position;
                            PointType::Print(pfp, p ) << '\n';
                            p += pa.nodes[i]->V/2;
                            PointType::Print(pfp, p ) << '\n' << '\n';

                        }
                    }
                }
                
                {
                    const string  sfn = s_pfx + PID + '_' + TID + "_n.dat";
                    const string  pfn = p_pfx + PID + '_' + TID + "_n.dat";
                    ios::ocstream sfp(sfn);
                    ios::ocstream pfp(pfn);
                    for(size_t i=1;i<=np;++i)
                    {
                        {
                            Vertex p = sa.nodes[i]->point->position;
                            PointType::Print(sfp, p ) << '\n';
                            p += sa.nodes[i]->basis.n/2;
                            PointType::Print(sfp, p ) << '\n' << '\n';
                            
                        }
                        
                        {
                            Vertex p = pa.nodes[i]->point->position;
                            PointType::Print(pfp, p ) << '\n';
                            p += pa.nodes[i]->basis.n/2;
                            PointType::Print(pfp, p ) << '\n' << '\n';
                            
                        }
                    }
                }
                
                

                sa.compile();
                pa.compile();

                {
                    const string  sfn = s_pfx + PID + '_' + TID + "_i.dat";
                    ios::ocstream sfp(sfn);
                    for(mutable_type x=1; x <= np; x += mutable_type(0.01) )
                    {
                        const Vertex v = sa(x);
                        PointType::Print(sfp,v) << '\n';
                    }
                }


                {
                    const string  sfn = s_pfx + PID + '_' + TID + "_va.dat";
                    ios::ocstream sfp(sfn);
                    Vertex         v,a;
                    for(mutable_type x=1; x <= np; x += mutable_type(0.01) )
                    {
                        (void)sa(x,&v,&a);
                        sfp("%.15g %.15g %.15g\n", x, sqrt_of(aliasing::cast<VTX,Vertex>(v).norm2()), sqrt_of(aliasing::cast<VTX,Vertex>(a).norm2()) );
                    }
                }


                
                {
                    const string  pfn = p_pfx + PID + '_' + TID + "_i.dat";
                    ios::ocstream pfp(pfn);
                    for(mutable_type x=1; x <= np+1; x += mutable_type(0.01) )
                    {
                        const Vertex v = pa(x);
                        PointType::Print(pfp,v) << '\n';
                    }
                }

                {
                    const string  pfn = p_pfx + PID + '_' + TID + "_va.dat";
                    ios::ocstream pfp(pfn);
                    Vertex         v,a;
                    for(mutable_type x=1; x <= np+1; x += mutable_type(0.01) )
                    {
                        (void)pa(x,&v,&a);
                        pfp("%.15g %.15g %.15g\n", x, sqrt_of(aliasing::cast<VTX,Vertex>(v).norm2()), sqrt_of(aliasing::cast<VTX,Vertex>(a).norm2()) );
                    }
                }

                const_type ftol(1e-5);
                {
                    typename numeric<mutable_type>::function F( &sa, &  Arc<T,POINT>::speed );
                    const_type slen = integrate::compute(F,const_type(1),const_type(np),ftol);
                    std::cerr << "slen=" << slen << std::endl;
                }

                {
                    typename numeric<mutable_type>::function F( &sa, &  Arc<T,POINT>::speed );
                    const_type plen = integrate::compute(F,const_type(1),const_type(np+1),ftol);
                    std::cerr << "plen=" << plen << std::endl;
                }
            }


        }
    };
    
}

#define _EUCLIDEAN(TYPE,POINT) euclidean_test<TYPE,POINT>::make(#TYPE,#POINT,nn)

#include "y/string/convert.hpp"

Y_UTEST(euclidean)
{

    size_t nn = 0;
    if(argc>1)
    {
        nn = string_convert::to<size_t>( argv[1], "nn" );
    }
    _EUCLIDEAN(float,point2d);
    _EUCLIDEAN(double,point2d);

    _EUCLIDEAN(float,point3d);
    _EUCLIDEAN(double,point3d);

    _EUCLIDEAN(float,complex);
    _EUCLIDEAN(double,complex);
    
}
Y_UTEST_DONE()



