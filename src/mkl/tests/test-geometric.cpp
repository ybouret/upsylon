#include "y/mkl/geometric/arc/periodic.hpp"
#include "y/mkl/geometric/arc/standard.hpp"
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

#if 1
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
    static inline void testPoints(const bool save=false)
    {

        typedef Point<T,VTX>                PointType;
        typedef typename PointType::Pointer PointerType;
        typedef typename PointType::vertex  vertex;

        std::cerr << "points: " << type_name_of<vertex>() << std::endl;
        std::cerr << "  dims: " << PointType::Dimensions << std::endl;

        typedef hash_set<PointKey,PointerType> Points;

        Points       points;
        const size_t np = 1+alea.leq(20);
        const vertex center = support::get<vertex>();
        const vertex radius = support::get<vertex>();
        const T      t0     = numeric<T>::two_pi * alea.to<T>();
        for(size_t i=0;i<np;++i)
        {
            const T theta     = (numeric<T>::two_pi*i)/np + t0;
            const T *r        = (const T *)&radius;
            const T *c        = (const T *)&center;
            const T  scale    = (1+T(0.5)*square_of(cos_of(theta)));
            const T  coord[3] = { c[0] + r[0] * scale * cos_of(theta), c[1] + r[1] * scale * sin_of(theta), c[2]*cos_of(theta) };
            PointerType p = new PointType();
            memcpy( (void*) & **p, coord, sizeof(T) * PointType::Dimensions );
            Y_ASSERT(points.insert(p));
        }
        std::cerr << "points=" <<  points << std::endl;


        StandardArc<T,VTX> sa;
        PeriodicArc<T,VTX> pa;
        for(typename Points::iterator it=points.begin();it!=points.end();++it)
        {
            sa.insert( *it );
            pa.insert( *it );
        }


        std::cerr << "sa: nodes=" << sa.nodes.size() << " segments=" << sa.segments.size() << std::endl;
        std::cerr << "pa: nodes=" << pa.nodes.size() << " segments=" << pa.segments.size() << std::endl;

        pa.buildP();
        if(save)
        {
            {
                const string  fileName = "pa-" + type2file( typeid(vertex) ) + "p.dat";
                ios::ocstream fp(fileName);
                for(size_t i=1;i<=pa.nodes.size();++i)
                {
                    PointType::Print(fp,***pa.nodes[i]) << '\n';
                }
                if(pa.nodes.size())
                {
                    PointType::Print(fp,***pa.nodes[1]) << '\n';
                }
            }

            {
                const string fileName = "pa-" + type2file( typeid(vertex) ) + "pp.dat";
                ios::ocstream fp(fileName);
                for(size_t i=1;i<=pa.segments.size();++i)
                {
                    const Segment<T,VTX> &segment = pa.segments[i];
                    for(T tau=0;tau<=1.0;tau+=0.1)
                    {
                        const vertex P = segment.P(tau);
                        PointType::Print(fp,P) << '\n';
                    }
                }
            }

            {
                const string  fileName = "pa-" + type2file( typeid(vertex) ) + "v.dat";
                ios::ocstream fp(fileName);

                const string  fileName2 = "pa-" + type2file( typeid(vertex) ) + "vn.dat";
                ios::ocstream fp2(fileName2);
                for(size_t i=1;i<=pa.segments.size();++i)
                {
                    const Segment<T,VTX> &segment = pa.segments[i];
                    for(T tau=0;tau<=1.0;tau+=0.1)
                    {
                        const vertex P = segment.P(tau);
                        const vertex V = segment.V(tau);

                        PointType::Print(fp,P)   << '\n';
                        PointType::Print(fp,P+V) << '\n';
                        fp << '\n';
                        fp2("%g %g\n", T(i)+tau, sqrt_of(PointType::Norm2(V)) );
                    }
                }
            }

             {
                const string  fileName = "pa-" + type2file( typeid(vertex) ) + "a.dat";
                ios::ocstream fp(fileName);
                const string  fileName2 = "pa-" + type2file( typeid(vertex) ) + "an.dat";
                ios::ocstream fp2(fileName2);
                for(size_t i=1;i<=pa.segments.size();++i)
                {
                    const Segment<T,VTX> &segment = pa.segments[i];
                    for(T tau=0;tau<=1.0;tau+=0.1)
                    {
                        const vertex P = segment.P(tau);
                        const vertex A = segment.A(tau);

                        PointType::Print(fp,P) << '\n';
                        PointType::Print(fp,P+A) << '\n';
                        fp << '\n';

                        fp2("%g %g\n", T(i)+tau, sqrt_of(PointType::Norm2(A)) );

                    }
                }
            }
        }

        pa.buildQ();
#if 1
        if(save)
        {
            {
                const string fileName = "pa-" + type2file( typeid(vertex) ) + "pq.dat";
                ios::ocstream fp(fileName);
                for(size_t i=1;i<=pa.segments.size();++i)
                {
                    const Segment<T,VTX> &segment = pa.segments[i];
                    for(T tau=0;tau<=1.0;tau+=0.1)
                    {
                        const vertex P = segment.Q(tau);
                        PointType::Print(fp,P) << '\n';
                    }
                }
            }

            {
                const string  fileName = "pa-" + type2file( typeid(vertex) ) + "vq.dat";
                ios::ocstream fp(fileName);

                const string  fileName2 = "pa-" + type2file( typeid(vertex) ) + "vqn.dat";
                ios::ocstream fp2(fileName2);
                for(size_t i=1;i<=pa.segments.size();++i)
                {
                    const Segment<T,VTX> &segment = pa.segments[i];
                    for(T tau=0;tau<=1.0;tau+=0.1)
                    {
                        const vertex P = segment.Q(tau);
                        const vertex V = segment.VQ(tau);

                        PointType::Print(fp,P)   << '\n';
                        PointType::Print(fp,P+V) << '\n';
                        fp << '\n';
                        fp2("%g %g\n", T(i)+tau, sqrt_of(PointType::Norm2(V)) );
                    }
                }
            }

            {
                const string  fileName = "pa-" + type2file( typeid(vertex) ) + "aq.dat";
                ios::ocstream fp(fileName);
                const string  fileName2 = "pa-" + type2file( typeid(vertex) ) + "aqn.dat";
                ios::ocstream fp2(fileName2);
                for(size_t i=1;i<=pa.segments.size();++i)
                {
                    const Segment<T,VTX> &segment = pa.segments[i];
                    for(T tau=0;tau<=1.0;tau+=0.1)
                    {
                        const vertex P = segment.Q(tau);
                        const vertex A = segment.AQ(tau);

                        PointType::Print(fp,P) << '\n';
                        PointType::Print(fp,P+A) << '\n';
                        fp << '\n';

                        fp2("%g %g\n", T(i)+tau, sqrt_of(PointType::Norm2(A)) );

                    }
                }
            }

        }
#endif

        std::cerr << "sizes: " << std::endl;
        Y_UTEST_SIZEOF(vertex);
        Y_UTEST_SIZEOF(PointType);
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
    testPoints<double,complex>(true);

}
Y_UTEST_DONE()

