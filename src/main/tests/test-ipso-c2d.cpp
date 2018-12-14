#include "y/ipso/contour2d.hpp"
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

Y_UTEST(ipso_c2d)
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
        ipso::contour2d::scan(d, 0, 0, 0, 0, x, y, z, NULL,NULL);
    }

    {
        ios::ocstream fp("contour2d.dat");
        const size_t  n = 101;
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
        ipso::contour2d::scan(d, 1, n, 1, n, x, y, z, linecb,&fp);

        ipso::contour2d::iso_segments segs;

        segs.ld(d, 1, n, 1, n, x, y, z);
        std::cerr << "#segs=" << segs.size() << std::endl;
        for(ipso::contour2d::iso_segments::iterator i=segs.begin();i!=segs.end();++i)
        {
            const ipso::contour2d::segments &seg = **i;
            std::cerr << "seg@index" << seg.indx << " : " << seg.size << std::endl;
        }
    }
}
Y_UTEST_DONE()


