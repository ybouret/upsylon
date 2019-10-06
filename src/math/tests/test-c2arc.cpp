#include "y/type/point2d.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"
#include "y/string.hpp"

using namespace upsylon;

namespace {
    typedef point2d<double> vtx_t;

    static inline void doArc(const vtx_t V0, const vtx_t V1, const unsigned idx)
    {
        const vtx_t P0(0.1,0.2);
        const vtx_t P1(1.1,0.34);

        const vtx_t D    = 6*(P1-P0)-3*(V0+V1);
        const vtx_t A0   = D;
        const vtx_t A1   = -D;
        const vtx_t beta = 6*(V1-V0);

        {
            const string  afn = vformat("a%u.dat",idx);
            ios::ocstream afp(afn);
            afp("%g %g\n", P0.x, P0.y);
            afp("%g %g\n", P1.x, P1.y);

        }
        const string  pfn = vformat("p%u.dat",idx);
        const string  vfn = vformat("v%u.dat",idx);
        ios::ocstream pfp(pfn);
        ios::ocstream vfp(vfn);

        std::cerr << "P0=" << P0 << ", P1=" << P1 << std::endl;
        std::cerr << "V0=" << V0 << ", V1=" << V1 << std::endl;
        std::cerr << "A0=" << A0 << ", A1=" << A1 << std::endl;

        for(double t=0.0;t<=1.0;t+=0.1)
        {
            const double t2  = t*t;
            const double t3  = t*t2;
            const double t4  = t2*t2;

            const double tt2  = t2/2;
            const double ttt3 = t3/3;
            const double ttt6 = t3/6;

            const vtx_t Vt = V0 + (t-tt2)*A0 + tt2*A1 + beta*(tt2-ttt3);
            const vtx_t Pt = P0 + t*V0 + (tt2-ttt6) * A0 + ttt6 * A1 + (ttt6-t4/12) * beta;

            pfp("%g %g\n",Pt.x,Pt.y);
            vfp("%g %g\n%g %g\n\n",Pt.x,Pt.y,Pt.x+Vt.x,Pt.y+Vt.y);
        }

    }

}


Y_UTEST(c2arc)
{

    doArc( 0.2*vtx_t(1,1), 0.2*vtx_t(1,0), 0 );
    doArc( 0.2*vtx_t(1,1), 0.2*vtx_t(1,1), 1 );





}
Y_UTEST_DONE()

