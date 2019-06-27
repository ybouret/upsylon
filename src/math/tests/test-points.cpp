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
    inline  dummy2( T a=0, T b=0) : u(a), v(b) {}
    //inline  dummy2(const dummy2 &other) throw() : u(other.u), v(other.v) {}
    //inline ~dummy2() throw() {}

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
    typedef Geometric::Curve<T,POINT>            CurveType;
    typedef typename CurveType::Node             CurveNode;
    std::cerr << "\tsizeof(T)         = " << sizeof(T)         << std::endl;
    std::cerr << "\tDimension         = " << PointInfo::Dimension << std::endl;
    std::cerr << "\tsizeof(PointType) = " << sizeof(PointType) << std::endl;
    std::cerr << "\tsizeof(CorePoint) = " << sizeof(CorePoint) << std::endl;
    std::cerr << "\tsizeof(CurveType) = " << sizeof(CurveType) << std::endl;
    std::cerr << "\tsizeof(CurveNode) = " << sizeof(CurveNode) << std::endl;

    const string suffix = vformat("%ud.dat", unsigned( PointInfo::Dimension ));

    PointsType points;
    const float noise   = 0.00f;
    const float xradius = 5.0f;
    const float yradius = 5.0f;

    for(size_t i=0;i<np;++i)
    {
        const float theta = ((numeric<float>::two_pi*i)/np) * (1.0f+noise*alea.symm<float>() );
        const float x     = 8.0f + xradius * cosf( theta ) * (1.0f+noise*alea.symm<float>() );
        const float y     = yradius * sinf( theta ) * (1.0f+noise*alea.symm<float>() );
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


    CurveType curve;

    curve. template compute< SEQUENCE< POINT<T> > >(points,Geometric::Periodic);

    {
        const string  fn = "periodic-curve" + suffix;
        ios::ocstream fp(fn);
        for(size_t i=1;i<=np;++i)
        {
            const PointType  &p    = points[i];
            CorePoint         c    = PointInfo::Type2Core(p);
            const CurveNode  &node = curve.nodes[i];
            Y_ASSERT((c-node.r).norm2()<=0);
            PointInfo::SaveType(fp,p) << '\n';
            c += node.celerity * node.t;
            PointInfo::SaveCore(fp,c) << '\n' << '\n';
        }
    }

    {
        const string  fn = "periodic-approx" + suffix;
        ios::ocstream fp(fn);
        const T step = T(0.05);
        const T xmax = T(np)+step;
        const T xmin = -step;
        for(T x=xmin;x<=xmax;x+=step)
        {
            PointType p,dp;
            curve.get(x,&p,&dp);
            PointInfo::SaveType(fp,p) << '\n';
            const CorePoint c = PointInfo::Type2Core(p)+PointInfo::Type2Core(dp);
            PointInfo::SaveCore(fp,c) << '\n' << '\n';
        }
    }


    {
        const string  fn = "periodic-angle" + suffix;
        ios::ocstream fp(fn);
        for(size_t i=1;i<=np;++i)
        {
            //const PointType  &p    = points[i];
            //CorePoint         c    = PointInfo::Type2Core(p);
            const CurveNode  &node = curve.nodes[i];
            PointInfo::SaveCore( fp("%u ", unsigned(i)),node.t)(" %15g\n", node.celerity);
        }
    }


    {
        const string  fn = "periodic-normal" + suffix;
        ios::ocstream fp(fn);
        for(size_t i=1;i<=np;++i)
        {
            const PointType  &p    = points[i];
            CorePoint         c    = PointInfo::Type2Core(p);
            const CurveNode  &node = curve.nodes[i];
            PointInfo::SaveType(fp,p) << '\n';
            c += node.n;
            PointInfo::SaveCore(fp,c) << '\n' << '\n';
        }
    }

    {
        const string  fn = "periodic-curvature" + suffix;
        ios::ocstream fp(fn);
        for(size_t i=1;i<=np;++i)
        {
            const CurveNode  &node = curve.nodes[i];
            fp("%u %g\n", unsigned(i), node.curvature);
        }
    }


    {
        const string  fn = "periodic-speed" + suffix;
        ios::ocstream fp(fn);
        const T step = T(0.05);
        const T xmax = T(np)+step;
        const T xmin = -step;
        for(T x=xmin;x<=xmax;x+=step)
        {
            CorePoint s = curve.speed_(x);
            fp("%g %g\n",x,sqrt_of(s.norm2()));
        }
    }



    curve. template  compute< SEQUENCE< POINT<T> > >(points,Geometric::Standard);
    {
        const string  fn = "standard-curve" + suffix;
        ios::ocstream fp(fn);
        for(size_t i=1;i<=np;++i)
        {
            const PointType  &p    = points[i];
            CorePoint         c    = PointInfo::Type2Core(p);
            const CurveNode  &node = curve.nodes[i];
            Y_ASSERT((c-node.r).norm2()<=0);
            PointInfo::SaveType(fp,p) << '\n';
            c += node.t * node.celerity;
            PointInfo::SaveCore(fp,c) << '\n' << '\n';
        }
    }

    {
        const string  fn = "standard-approx" + suffix;
        ios::ocstream fp(fn);
        const T step = T(0.05);
        const T xmax = T(np)+step;
        const T xmin = -step;
        for(T x=xmin;x<=xmax;x+=step)
        {
            PointType p,dp;
            curve.get(x,&p,&dp);
            PointInfo::SaveType(fp,p) << '\n';
            const CorePoint c = PointInfo::Type2Core(p)+PointInfo::Type2Core(dp);
            PointInfo::SaveCore(fp,c) << '\n' << '\n';

        }
    }

    {
        const string  fn = "standard-speed" + suffix;
        ios::ocstream fp(fn);
        const T step = T(0.05);
        const T xmax = T(np)+step;
        const T xmin = -step;
        for(T x=xmin;x<=xmax;x+=step)
        {
            CorePoint s = curve.speed_(x);
            fp("%g %g\n",x,sqrt_of(s.norm2()));
        }
    }


    {
        const string  fn = "standard-angle" + suffix;
        ios::ocstream fp(fn);
        for(size_t i=1;i<=np;++i)
        {
            //const PointType  &p    = points[i];
            //CorePoint         c    = PointInfo::Type2Core(p);
            const CurveNode  &node = curve.nodes[i];
            PointInfo::SaveCore( fp("%u ", unsigned(i)),node.t)(" %15g\n", node.celerity);
        }
    }

    {
        const string  fn = "standard-normal" + suffix;
        ios::ocstream fp(fn);
        for(size_t i=1;i<=np;++i)
        {
            const PointType  &p    = points[i];
            CorePoint         c    = PointInfo::Type2Core(p);
            const CurveNode  &node = curve.nodes[i];
            PointInfo::SaveType(fp,p) << '\n';
            c += node.n;
            PointInfo::SaveCore(fp,c) << '\n' << '\n';
        }
    }

    {
        const string  fn = "standard-curvature" + suffix;
        ios::ocstream fp(fn);
        for(size_t i=1;i<=np;++i)
        {
            const CurveNode  &node = curve.nodes[i];
            fp("%u %g\n", unsigned(i), node.curvature);
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


