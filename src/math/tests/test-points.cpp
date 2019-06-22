#include "y/math/geometric/ops.hpp"
#include "y/math/geometric/curve.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/list.hpp"
#include "y/memory/pooled.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace math;

template <typename T> struct dummy2
{
    T u, v;
    inline dummy2( T a=0, T b=0) : u(a), v(b)
    {
    }

    friend inline std::ostream &operator<<( std::ostream &os, const dummy2 &d )
    {
        os << "{" << d.u << "," << d.v<< "}";
        return os;
    }
};

template <typename T> struct dummy3
{
    T u, v, w;
    inline dummy3( T a=0, T b=0, T c=0) : u(a), v(b), w(c)
    {
    }

    friend inline std::ostream &operator<<( std::ostream &os, const dummy3 &d )
    {
        os << "{" << d.u << "," << d.v<< "," << d.w <<  "}";
        return os;
    }
};



template <typename T,
template <typename> class POINT,
template <typename> class SEQUENCE>
static inline void do_points(const size_t np)
{
    std::cerr << "Perform Tests On Points" << std::endl;
    typedef Geometric::Points<T,POINT,SEQUENCE>  PointsType;
    typedef Geometric::PointInfoFor<T,POINT>     PointInfo;
    typedef typename PointInfo::Type             PointType;
    typedef typename PointInfo::Core             CorePoint;
    
    PointsType points;
    const float noise = 0.01f;
    const float radius = 5.0;
    for(size_t i=0;i<np;++i)
    {
        const float theta = ((numeric<float>::two_pi*i)/np) * (1.0f+noise*alea.symm<float>() );
        const float x     = radius * cosf( theta ) * (1.0f+noise*alea.symm<float>() );
        const float y     = radius * sinf( theta ) * (1.0f+noise*alea.symm<float>() );
        points.add(x,y);
    }

    std::cerr << "Points: " << std::endl;
    for(typename PointsType::iterator i=points.begin(); i != points.end(); ++i)
    {
        const PointType &p = *i;
        const CorePoint &c = PointInfo::Type2Core(p);
        std::cerr << "\t" << p << " / " <<  c << std::endl;
    }
    
    std::cerr << "Barycenter: " << Geometric::Ops::Barycenter(points) << std::endl;
    std::cerr << std::endl;

    typedef Geometric::Curve<T,POINT> CurveType;
    typedef typename CurveType::Node  CurveNode;
    CurveType curve;

    curve. template compute< SEQUENCE< POINT<T> > >(points,Geometric::Periodic);

    {
        ios::ocstream fp("curve-periodic.dat");
        for(size_t i=1;i<=np;++i)
        {
            const PointType  &p    = points[i];
            CorePoint         c    = PointInfo::Type2Core(p);
            const CurveNode  &node = curve.nodes[i];
            fp("%g %g\n", c.x, c.y);
            c += node.t;
            fp("%g %g\n\n", c.x, c.y);
        }
    }

    {
        ios::ocstream fp("curvature.dat");
        for(size_t i=1;i<=np;++i)
        {
            const CurveNode  &node = curve.nodes[i];
            fp("%u %g\n", unsigned(i), node.curvature);
        }
    }



    curve. template  compute< SEQUENCE< POINT<T> > >(points,Geometric::Standard);
    {
        ios::ocstream fp("curve-standard.dat");
        for(size_t i=1;i<=np;++i)
        {
            const PointType  &p    = points[i];
            CorePoint         c    = PointInfo::Type2Core(p);
            const CurveNode  &node = curve.nodes[i];
            fp("%g %g\n", c.x, c.y);
            c += node.t;
            fp("%g %g\n\n", c.x, c.y);
        }
    }



}

#define DO_POINTS__(TYPE,SEQ)    \
do_points<TYPE,point2d,SEQ>(np); \
do_points<TYPE,complex,SEQ>(np); \
do_points<TYPE,point3d,SEQ>(np); \
do_points<TYPE,dummy2,SEQ>(np);  \
do_points<TYPE,dummy3,SEQ>(np)

#define DO_POINTS_(SEQ) \
DO_POINTS__(float,SEQ); \
DO_POINTS__(double,SEQ)

#include "y/string/convert.hpp"

Y_UTEST(points)
{
    size_t np=3;
    if(argc>1)
    {
        np = string_convert::to<size_t>( argv[1], "np" );
    }
    DO_POINTS_(list);
    DO_POINTS_(vector_for<memory::global>::device);
    DO_POINTS_(vector_for<memory::pooled>::device);

}
Y_UTEST_DONE()


