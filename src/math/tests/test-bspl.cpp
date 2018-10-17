#include "y/math/signal/b-splines.hpp"
#include "y/utest/run.hpp"
#include "y/type/point3d.hpp"
#include "y/sequence/vector.hpp"
#include "y/ios/ocstream.hpp"
#include "support.hpp"

using namespace upsylon;
using namespace math;

namespace
{
    template <typename VTX>
    static inline
    ios::ostream & write_vtx( ios::ostream &fp, const VTX &v )
    {
        static const size_t       dim = sizeof(VTX)/sizeof(typename VTX::type);
        const typename VTX::type *p = (const typename VTX::type *)&v;
        fp("%g",p[0]);
        for(size_t j=1;j<dim;++j) fp(" %g",p[j]);
        return fp;
    }

    template <typename T,typename U>
    static inline void do_test()
    {
        vector<U> points;

        {
            U tmp;
            points << tmp;
            for(size_t i=1+alea.leq(10);i>0;--i)
            {
                tmp += support::get<U>();
                points << tmp;
            }
        }

        const unsigned dim = sizeof(U)/sizeof(T);
        {
            ios::ocstream fp( vformat("curv%u.dat",dim) );
            for(size_t i=1;i<=points.size();++i)
            {
                write_vtx(fp,points[i]) << "\n";
            }
        }

        {
            ios::ocstream fp( vformat("bspl%u.dat",dim) );
            for(T x=0;x<=T(1);x+=T(0.001))
            {
                const U v = Cubic_Bsplines(x,points);
                write_vtx(fp,v) << "\n";
            }
        }

    }

    template <typename T,
    template <class> class VTX>
    static inline void do_approx()
    {
        CubicApproximation<T,VTX> approx;
        typedef typename CubicApproximation<T,VTX>::vertex vtx;
        {
            vtx tmp;
            approx << tmp;
            for(size_t i=1+alea.leq(10);i>0;--i)
            {
                tmp += support::get<vtx>();
                approx << tmp;
            }
        }

        const unsigned dim = approx.DIM;
        {
            ios::ocstream fp( vformat("approx%u.dat",dim) );
            for(size_t i=1;i<=approx.size();++i)
            {
                write_vtx(fp,approx[i]) << "\n";
            }
        }

        {
            ios::ocstream fp( vformat("bezier%u.dat",dim) );
            for(T x=0;x<=T(1);x+=T(0.001))
            {
                const vtx v = approx(x);
                write_vtx(fp,v) << "\n";
            }
        }

        T max_speed = 0;
        {
            ios::ocstream fp( vformat("speed%u.dat",  dim) );
            for(T x=0;x<=T(1);x+=T(0.001))
            {
                const T s = approx.speed(x);
                fp("%g %g\n",x,s);
                if(s>max_speed) max_speed=s;
            }
        }

        {
            ios::ocstream fp(  vformat("tangent%u.dat",dim) );
            for(T x=0;x<=T(1);x+=T(0.001))
            {
                const vtx p = approx(x);
                const vtx v = approx.tangent(x)/max_speed;
                write_vtx(fp,p) << "\n";
                write_vtx(fp,v+p) << "\n";
                fp << "\n";
            }
        }

        std::cerr << "arc length=" << approx.arc_length(0,1) << std::endl;




    }

}


Y_UTEST(bspl)
{

    do_test< float,point2d<float> >();
    do_test< double,point3d<double> >();

    do_approx<double,point2d>();
    do_approx<float,point3d>();


}
Y_UTEST_DONE()


