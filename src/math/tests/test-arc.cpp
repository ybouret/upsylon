#include "y/math/signal/arc.hpp"
#include "y/utest/run.hpp"
#include "y/string/convert.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace math;

namespace
{

    template <typename CURVE>
    static inline void handle_curve( const CURVE &C, const string &id )
    {

        typedef typename CURVE::point_type POINT;
        arc::standard_spline<POINT> NS;
        arc::standard_spline<POINT> ZS;
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
            const float radius = 1.0f+noise*alea.symm<float>();
            const float x = radius * cosf(theta);
            const float y = radius * sinf(theta);
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

}
Y_UTEST_DONE()

