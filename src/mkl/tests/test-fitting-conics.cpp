

#include "y/mkl/fitting/built-in/conics.hpp"
#include "y/mkl/types.hpp"

#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace mkl;
using namespace fitting;

namespace  {
    
    static inline
    void save_ellipse(const char           *filename,
                      const point2d<double> &center,
                      const point2d<double> &radius,
                      const matrix<double>  &rotate)
    {
        ios::ocstream fp(filename);
        
        const size_t N = 100;
        for(size_t i=0;i<=N;++i)
        {
            const double theta = (numeric<double>::two_pi * i)/N;
            const double c     = cos(theta);
            const double s     = sin(theta);
            
            point2d<double> r(c*radius.x,s*radius.y);
            rotate(r);

            r += center;
            fp("%g %g\n",r.x,r.y);
        }
    
    }
    
    
}

Y_UTEST(fitting_conics)
{

    built_in::iConics IC;
    built_in::dConics DC;

    IC.ellipse();
    DC.ellipse();

    const double ra = 10 + 100 * alea.to<double>();
    const double rb = 10 + 100 * alea.to<double>();
    const double xc = 200 * alea.symm<double>();
    const double yc = 200 * alea.symm<double>();

    const double th =  numeric<double>::two_pi * alea.to<double>();
    const double cc = cos(th);
    const double ss = sin(th);


    std::cerr << "xc=" << xc << std::endl;
    std::cerr << "yc=" << yc << std::endl;

    std::cerr << "ra=" << ra << std::endl;
    std::cerr << "rb=" << rb << std::endl;
    std::cerr << std::endl;
    
    ios::ocstream::overwrite("edat.dat");
    for(size_t i=10+alea.leq(10);i>0;--i)
    {
        const double phi = numeric<double>::two_pi * alea.to<double>();
        const double x0  = ra * cos(phi);
        const double y0  = rb * sin(phi);
        const double x   = xc + cc*x0 - ss * y0;
        const double y   = yc + ss*x0 + cc * y0;
        const unit_t X   = unit_t(x);
        const unit_t Y   = unit_t(y);
        ios::ocstream::echo("edat.dat","%ld %ld\n", long(X), long(Y) );
        IC.add(X,Y);
        DC.add(double(X),double(Y));
    }

    point2d<double> center;
    point2d<double> radius;
    matrix<double>  rotate;
    
    if( IC.ellipse(center,radius,rotate) )
    {
        std::cerr << "center: " << center << std::endl;
        std::cerr << "radius: " << radius << std::endl;
        std::cerr << "rotate: " << rotate << std::endl;
        std::cerr << std::endl;
        save_ellipse("iell.dat",center,radius,rotate);

        if(false)
        {
            ios::ocstream fp("gell.dat");
            for(int x=-300;x<=300;++x)
            {
                for(int y=-300;y<=300;++y)
                {
                    if( IC.at(x,y) >= 0)
                    {
                        fp("%d %d\n",x,y);
                    }
                }
            }
        }
    }

    
    if( DC.ellipse(center,radius,rotate) )
    {
        std::cerr << "center: " << center << std::endl;
        std::cerr << "radius: " << radius << std::endl;
        std::cerr << "rotate: " << rotate << std::endl;
        std::cerr << std::endl;
        save_ellipse("dell.dat",center,radius,rotate);
    }
    
    
}
Y_UTEST_DONE()

