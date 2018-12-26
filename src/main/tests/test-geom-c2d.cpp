#include "y/geometry/contour2d.hpp"
#include "y/container/matrix.hpp"
#include "y/ipso/field2d.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace geometry;

namespace
{
    static inline
    void callback(double x0, double y0, double x1, double y1, size_t index, double level, void *args )
    {
        assert(args);
        ios::ocstream &fp = *static_cast<ios::ocstream *>(args);
        fp("%g %g %u %g\n",   x0,y0, unsigned(index), level );
        fp("%g %g %u %g\n\n", x1,y1, unsigned(index), level );

    }
}

Y_UTEST(geom_c2d)
{


    const size_t   nx=60;
    const size_t   ny=80;
    matrix<double> M(ny,nx);
    vector<double> X(nx), Y(ny);
    for(size_t j=1;j<=ny;++j)
    {
        Y[j] = double(j-1)/(ny-1);
        const double yy = Y[j] - 0.5;
        for(size_t i=1;i<=nx;++i)
        {
            X[i] = double(i-1)/(nx-1);
            const double xx1 = X[i]-0.8;
            const double xx2 = X[i]-0.2;
            const double r1 = math::__hypotenuse(yy,xx1);
            const double r2 = math::__hypotenuse(yy,xx2);
            const double V  = 1.0/(r1+0.1)-1.0/(r2+0.1);
            M[j][i] = V;
        }
    }

    vector<double> z(16,as_capacity);
    

    z.push_back(-5);
    z.push_back(-2);
    z.push_back(-0.1);
    z.push_back(0);
    z.push_back(0.1);
    z.push_back(2);
    z.push_back(5);

    std::cerr << "Contours for z=" << z << std::endl;
    {
        ios::ocstream fp("c2d.dat");
        contour2d::scan( M, 1, nx, 1, ny, X, Y, z, callback, &fp);
    }

    std::cerr << "plot 'c2d.dat' with lines linecolor variable" << std::endl;

    {
        contour2d::level_sets iso( z.size() );
        std::cerr << "#iso=" << iso.size() << std::endl;
        contour2d::scan( M, 1, nx, 1, ny, X, Y, z, contour2d::level_sets::call, &iso );
        std::cerr << "#iso=" << iso.size() << std::endl;
        for(contour2d::level_sets::iterator i=iso.begin(); i!=iso.end(); ++i)
        {
            const contour2d::level_set &ls = **i;
            std::cerr << "iso[" << ls.index << "] : " << ls.size << std::endl;
        }
    }


}
Y_UTEST_DONE()

