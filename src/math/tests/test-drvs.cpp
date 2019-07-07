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

    static inline
    double f3( double x, array<double> &param )
    {
        double ans = 0;
        for(size_t i=1;i<=param.size();++i)
        {
            ans += ipower(x,i-1) * param[i];
        }
        return ans;
    }
}


Y_UTEST(drvs)
{
    {
        derivative<double> drvs;
        double (*proc)(double) = sin;
        numeric<double>::function F = proc;
        ios::ocstream fp( "drvs.dat");
        ios::ocstream fp2("drv2.dat");

        for(double x=0;x<=3;x+=0.1)
        {
            fp( "%g %g %g %g\n", x, proc(x), drvs.diff(proc, x, 0.01), drvs.diff(F,x,0.01) );
            fp2("%g %g %g %g\n", x, proc(x), drvs.diff2(proc, x, 0.01), drvs.diff2(F,x,0.01) );
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
        for(float x=0;x<=3;x+=0.1f)
        {
            p[1] = x;
            for(float y=0;y<=3;y+=0.1f)
            {
                p[2] = y;
                const float value = f2(p);
                drvs.grad(g,f2,p,0.001f);
                fp("%g %g %g %g %g\n", x,y, value, g[1], g[2] );
                drvs.grad(g,F2,p,0.001f);
            }
            fp << '\n';
        }
    }

    {
        vector<double>      param(3); param[1] = 1.2; param[2] = 2.1; param[3] = 0.1;
        derivatives<double> drvs;
        for(double x=0;x<=1;x+=0.1)
        {
            std::cerr << "parametric_diff(" << x << ")=" << f3(x,param) << " -> " << drvs.parametric_diff(f3,x,param,0.001) << std::endl;
        }
    }

    {
        vector<double>     param(3); param[1] = 1.2; param[2] = 2.1; param[3] = 0.1;
        vector<double>     g( param.size() );
        derivatives<double> drvs;
        for(double x=0;x<=1;x+=0.1)
        {
            drvs.parametric_grad(g, f3, x, param, 0.001);
            std::cerr << "parametric_grad(" << x << "," << param << ")=" << f3(x,param) << " -> " << g << std::endl;
        }
    }

}
Y_UTEST_DONE()
