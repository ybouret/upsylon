#include "y/math/signal/arc.hpp"
#include "y/utest/run.hpp"
#include "y/string/convert.hpp"
#include "y/ios/ocstream.hpp"
#include <typeinfo>

using namespace upsylon;
using namespace math;

namespace
{

    template <typename CURVE>
    static inline void handle_curve( const CURVE &C, const string &id )
    {

        typedef typename CURVE::point_type POINT;
        typedef typename CURVE::real_type  REAL;
        std::cerr << "arc of <" << typeid(POINT).name() << ">, based on <" << typeid(REAL).name() << ">" << std::endl;

        arc::standard_spline<POINT> NS;
        arc::standard_spline<POINT> ZS(false,false);
        arc::periodic_spline<POINT> PS;

        {
            const string  out = id + ".dat";
            ios::ocstream fp(out);
            for(size_t i=1;i<=C.size();++i)
            {
                fp("%u ",unsigned(i));
                arc::save_point(fp,C[i]) << '\n';
            }
        }

        NS.compute(C);
        ZS.compute(C);
        PS.compute(C);

        {
            std::cerr << "\tnatural spline" << std::endl;
            const string out = id + "-ns.dat";
            ios::ocstream fp(out);
            for(REAL t=1;t<=C.size();t+=0.02)
            {
                fp("%.15g ",t);
                const POINT p = NS.position(t,C);
                arc::save_point(fp,p) << '\n';
            }
            std::cerr << "\t\textent: " << NS.extent(1,C.size(),C) << std::endl;
        }

        {
            std::cerr << "\tzero-derivative spline" << std::endl;
            const string  out = id + "-zs.dat";
            ios::ocstream fp(out);
            for(REAL t=1;t<=C.size();t+=0.02)
            {
                fp("%.15g ",t);
                const POINT p = ZS.position(t,C);
                arc::save_point(fp,p) << '\n';
            }
            std::cerr << "\t\textent: " << ZS.extent(1,C.size(),C) << std::endl;
        }

        {
            std::cerr << "\tperiodic spline" << std::endl;
            const string  out = id + "-ps.dat";
            ios::ocstream fp(out);
            for(REAL t=1;t<=C.size()+1;t+=0.02)
            {
                fp("%.15g ",t);
                const POINT p = PS.position(t,C);
                arc::save_point(fp,p) << '\n';
                volatile const REAL g = PS.curvature(t,C);
                (void)g;
            }
            std::cerr << "\t\textent: " << PS.extent(1,C.size()+1,C) << std::endl;
        }

    }

    // 2D
    template <typename CURVE>
    static inline void metrics_for( const CURVE &C, const string &id )
    {

        typedef typename CURVE::point_type POINT;
        typedef typename CURVE::real_type  REAL;
        std::cerr << "metrics for arc of <" << typeid(POINT).name() << ">, based on <" << typeid(REAL).name() << ">" << std::endl;

        {
            const string  out = id + ".dat";
            ios::ocstream fp(out);
            for(size_t i=1;i<=C.size();++i)
            {
                arc::save_point(fp,C[i]) << '\n';
            }
        }

        arc::standard_spline<POINT> NS;
        arc::standard_spline<POINT> ZS(false,false);
        arc::periodic_spline<POINT> PS;

        NS.compute(C);
        ZS.compute(C);
        PS.compute(C);

        {
            std::cerr << "\tnatural metrics" << std::endl;
            const string  out = id + "-ns.dat";
            ios::ocstream fp(out);
            for(REAL t=1;t<=C.size();t+=0.02)
            {
                POINT s;
                POINT p = NS.position(t,C,&s);
                arc::save_point(fp,p) << '\n';
                p+=s;
                arc::save_point(fp,p) << '\n';
                fp << '\n';
            }
        }

        {
            std::cerr << "\tzero-diff metrics" << std::endl;
            const string  out = id + "-zs.dat";
            ios::ocstream fp(out);
            for(REAL t=1;t<=C.size();t+=0.02)
            {
                POINT s;
                POINT p = ZS.position(t,C,&s);
                arc::save_point(fp,p) << '\n';
                p+=s;
                arc::save_point(fp,p) << '\n';
                fp << '\n';
            }
        }

        {
            std::cerr << "\tperiodic metrics" << std::endl;
            const string  out = id + "-ps.dat";
            ios::ocstream fp(out);
            for(REAL t=1;t<=C.size()+1;t+=0.02)
            {
                POINT s;
                POINT p = PS.position(t,C,&s);
                arc::save_point(fp,p) << '\n';
                p+=s;
                arc::save_point(fp,p) << '\n';
                fp << '\n';
            }
        }

        {
            std::cerr << "\twriting speed" << std::endl;
            const string  out = id + "-speed.dat";
            ios::ocstream fp(out);
            const REAL dt = 0.01;
            for(REAL t=REAL(1)-dt;t<=C.size()+1;t+=dt)
            {
                fp("%g ",t);
                fp(" %g", PS.speed(t,C));
                fp(" %g", NS.speed(t,C));
                fp(" %g", ZS.speed(t,C));
                fp << '\n';
            }
        }

        {
            std::cerr << "\twriting curvature" << std::endl;
            const string  out = id + "-curvature.dat";
            ios::ocstream fp(out);
            const REAL dt = 0.005;
            for(REAL t=REAL(1)-dt;t<=C.size()+1;t+=dt)
            {
                fp("%g ",t);
                fp(" %g", PS.curvature(t,C));
                fp(" %g", NS.curvature(t,C));
                //fp(" %g", ZS.curvature(t,C));
                fp << '\n';
            }
        }

        {
            std::cerr << "\twriting developped" << std::endl;
            const string  out = id + "-dev.dat";
            ios::ocstream fp(out);
            const REAL dt = 0.01;
            for(REAL t=1;t<=C.size()+1;t+=dt)
            {
                POINT       p = PS.position(t,C);
                POINT       u;
                const REAL  g = PS.curvature(t,C,&u);
                const POINT n = arc::tangent2normal<POINT,REAL>(u);
                p += n/g;
                arc::save_point(fp,p) << '\n';
            }
        }

    }

}

Y_UTEST(arc)
{

    arc::points<float>            c1f;
    arc::points< point2d<float> > c2f;
    arc::points< complex<float> > C2f;
    arc::points< point3d<float> > c3f;

    arc::points<double>            c1d;
    arc::points< point2d<double> > c2d;
    arc::points< complex<double> > C2d;
    arc::points< point3d<double> > c3d;

    size_t np = 3;
    if(argc>1)
    {
        np = string_convert::to<size_t>(argv[1],"np");
    }

    const float  dz    = 1.0/np;
    const float  noise = 0.01f;
    for(size_t i=0;i<np;++i)
    {

        {
            const float theta  = (i*numeric<float>::two_pi)/np*(1.0f+noise*alea.symm<float>());
            const float xradius = 1.0f+noise*alea.symm<float>();
            const float yradius = 0.8f+noise*alea.symm<float>();

            const float x = xradius * cosf(theta);
            const float y = yradius * sinf(theta);
            const float z = (dz * i) * (1.0f+noise*alea.symm<float>());

            c1f.add(x);
            c2f.add(x,y);
            C2f.add(x,y);
            c3f.add(x,y,z);

            c1d.add(x);
            c2d.add(x,y);
            C2d.add(x,y);
            c3d.add(x,y,z);
        }
    }

    handle_curve(c1f, "c1f");
    handle_curve(c2f, "c2f");
    handle_curve(C2f, "q2f");
    handle_curve(c3f, "c3f");

    handle_curve(c1d, "c1d");
    handle_curve(c2d, "c2d");
    handle_curve(C2d, "q2d");
    handle_curve(c3d, "c3d");

    metrics_for(c2f, "m2f");


}
Y_UTEST_DONE()

