#include "y/geometry/iso2d.hpp"
#include "y/utest/run.hpp"
#include "y/container/matrix.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace geometry;

Y_UTEST(geom_iso2d)
{
    if(false)
    {
        iso2d::shared_points db;
        for(size_t iter=10+alea.leq(100);iter>0;--iter)
        {
            const unit_t   i = 100 - unit_t( alea.leq(200) );
            const unit_t   j = 100 - unit_t( alea.leq(200) );
            const unsigned p = alea.partial<unsigned>(5);
            const iso2d::identifier   tag(i,j,p);
            const iso2d::vertex       vtx = alea.on_circle<double,point2d>();
            const iso2d::shared_point sp  = new iso2d::unique_point(tag,vtx);
            if(!db.insert(sp))
            {
                continue;
            }

        }
        std::cerr << "#unique_point=" << db.size() << std::endl;

    }

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
        //ios::ocstream fp("iso2d.dat");
        iso2d::levels L;
        iso2d::scan( L, M, 1, nx, 1, ny, X, Y, z);
        std::cerr << "#L=" << L.size() << std::endl;
        for(size_t i=1;i<=L.size();++i)
        {
            std::cerr << "#L[" << i << "]=" << L[i]->size() << std::endl;
        }
    }

    //std::cerr << "plot 'iso2d.dat' with lines linecolor variable" << std::endl;

}
Y_UTEST_DONE()

