#include "y/oxide/algo/contour2d.hpp"
#include "y/sequence/vector.hpp"
#include "y/ios/ocstream.hpp"
#include "y/container/matrix.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
static inline
void linecb(double x1,double y1,double x2,double y2,
            double level,
            size_t indx,
            void *args)
{
    assert(args);
    (void)level;
    (void)indx;
    ios::ostream &fp = *static_cast<ios::ocstream *>(args);
    fp("%g %g\n%g %g\n\n",x1,y1,x2,y2);
}

Y_UTEST(oxide_contour2d)
{

    const int      nc = 3;
    vector<double> z(nc);
    z[1] = -0.1;
    z[2] = 0;
    z[3] = 0.1;

    {
        double  **d = 0;
        double   *x = 0;
        double   *y = 0;
        Oxide::Contour2D::Scan(d, 0, 0, 0, 0, x, y, z, NULL,NULL);
    }

    {
        ios::ocstream  fp("contour2d.dat");
        const size_t   n = 101;
        matrix<float>  d(n,n);
        vector<double> x(n);
        vector<double> y(n);
        for(size_t i=1;i<=n;++i)
        {
            x[i] = y[i] = double(i-1)/(n-1);
        }

        for(size_t i=1;i<=n;++i)
        {
            for(size_t j=1;j<=n;++j)
            {
                d[i][j] = float(square_of((x[i]-0.2)/0.3) + square_of((y[j]-0.1)/0.1) - 1);
            }
        }
        Oxide::Contour2D::Scan(d, 1, n, 1, n, x, y, z, linecb,&fp);


    }
}
Y_UTEST_DONE()


