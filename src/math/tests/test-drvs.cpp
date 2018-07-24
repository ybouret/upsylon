#include "y/math/fcn/derivatives.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;
using namespace math;

namespace
{
    static inline
    float f2(const array<float> &u)
    {
        const float x = u[1];
        const float y = u[2];
        return 1.0f + sinf(2.0f*x) * cosf(3.0f*y);
    }
}


Y_UTEST(drvs)
{
    {
        derivative<double> drvs;
        double (*proc)(double) = sin;
        numeric<double>::function F = proc;
        ios::ocstream fp("drvs.dat");
        for(double x=0;x<=3;x+=0.1)
        {
            const double dd = drvs.diff(proc, x, 0.01);
            fp("%g %g %g %g\n", x, proc(x), dd, drvs.diff(F,x,0.01) );
        }
    }
    std::cerr << "unit_step_size: " << derivative<float>::unit_step_size() << ", " <<  derivative<double>::unit_step_size() << std::endl;
    {
        derivatives<float> drvs;
        ios::ocstream fp("grad.dat");
        vector<float> p(2);
        vector<float> g(2);
        float (*proc2)(const array<float> &) = f2;
        numeric<float>::scalar_field F2 = proc2;
        for(float x=0;x<=3;x+=0.1)
        {
            p[1] = x;
            for(float y=0;y<=3;y+=0.1)
            {
                p[2] = y;
                const float value = f2(p);
                drvs.grad(g,f2,p,0.001);
                fp("%g %g %g %g %g\n", x,y, value, g[1], g[2] );
                drvs.grad(g,F2,p,0.001);
            }
            fp << '\n';
        }
    }

}
Y_UTEST_DONE()
