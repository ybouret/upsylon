
#include "y/mkl/fitting/built-in/circle.hpp"

#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"
#include "y/mkl/kernel/lu.hpp"

using namespace upsylon;
using namespace mkl;
using namespace fitting;

typedef built_in::circle<unit_t,float> circle_type;
typedef circle_type::sampling          sample_type;
typedef circle_type::samples_type      samples_type;

namespace
{
    static void fill( sample_type &s, const float xc, const float yc, const float r )
    {
        for(size_t i=3+alea.leq(30);i>0;--i)
        {
            const float phi = numeric<float>::two_pi * alea.to<float>();
            const float x   = xc + r*cos(phi)*(1.0f+0.1f*alea.symm<float>());
            const float y   = yc + r*sin(phi)*(1.0f+0.1f*alea.symm<float>());
            s.append( unit_t(x), unit_t(y) );
        }
    }
    
    static inline void save_sample(const sample_type &s, ios::ostream &fp)
    {
        for(size_t i=1;i<=s.count();++i)
        {
            const point2d<unit_t> &p = s.abscissa[i];
            fp("%ld %ld\n", long(p.x), long(p.y));
        }
    }
    
}

Y_UTEST(fitting_circles)
{
    

    samples_type db("circles");
    
    sample_type &C1 = db( sample_type::create("C1") );
    sample_type &C2 = db( sample_type::create("C2") );
    
    std::cerr << C1.name << " and " << C2.name << std::endl;
    std::cerr << "in " << db.name << std::endl;
    
    fill(C1,9,-4,34); { ios::ocstream fp("c1.dat"); save_sample(C1,fp); }
    fill(C2,-2,7,34); { ios::ocstream fp("c2.dat"); save_sample(C2,fp); }
    circle_type CC;

    *C1 << "a" << "b" << "c";
    *C2 << "c" << "a" << "b";


    vector<float> a1(3,0);
    vector<float> a2(3,0);
    vector<bool>  used(3,true);

    C1.setup(a1);
    C2.setup(a2);
    const float C1D2 = C1.D2(CC.F,a1); std::cerr << "C1.D2=" << C1D2 << std::endl;
    const float C2D2 = C2.D2(CC.F,a2); std::cerr << "C2.D2=" << C2D2 << std::endl;

    {
        matrix<float> alpha(3,3);
        vector<float> beta(3,0);

        const float C1D2a = C1.D2(alpha, beta, CC.F, CC.G, a1, used);
        std::cerr << "C1.D2a=" << C1D2a << std::endl;
        std::cerr << "beta  =" << beta  << std::endl;
        std::cerr << "alpha =" << alpha << std::endl;
        if(LU::build(alpha))
        {
            LU::solve(alpha,beta);
            const double xa = beta[ *(*C1)["a"] ]/2;
            const double ya = beta[ *(*C1)["b"] ]/2;
            const double ca = beta[ *(*C1)["c"] ];
            const double ra = sqrt( max_of<double>(0,ca+xa*xa+ya*ya) );
            std::cerr << "xa=" << xa << ", ya=" << ya << ", ra=" << ra << std::endl;
        }

        const float C2D2a = C2.D2(alpha, beta, CC.F, CC.G, a1, used);
        std::cerr << "C2.D2a=" << C2D2a << std::endl;
        std::cerr << "beta  =" << beta  << std::endl;
        std::cerr << "alpha =" << alpha << std::endl;
        if(LU::build(alpha))
        {
            LU::solve(alpha,beta);
            const double xb = beta[ *(*C2)["a"] ]/2;
            const double yb = beta[ *(*C2)["b"] ]/2;
            const double cb = beta[ *(*C2)["c"] ];
            const double rb = sqrt( max_of<double>(0,cb+xb*xb+yb*yb) );
            std::cerr << "xb=" << xb << ", yb=" << yb << ", rb=" << rb << std::endl;
        }
    }

    circle_type::ls_type ls;
    vector<float> aerr(3,0);

    ls.verbose = true;
    
    if( ls.fit(C1,CC.F,CC.G,a1,used,aerr) )
    {
        const size_t NMAX=100;
        const float   xc = a1[ *(*C1)["a"] ]/2;
        const float   yc = a1[ *(*C1)["b"] ]/2;
        const float   cc = a1[ *(*C1)["c"] ];
        const float   r  = sqrt( max_of<float>(0,cc+xc*xc+yc*yc) );
        ios::ocstream fp("c1fit.dat");
        for(size_t i=0;i<=NMAX;++i)
        {
            const float phi = (i*numeric<float>::two_pi)/NMAX;
            fp("%g %g\n", xc + r*cosf(phi), yc+r*sinf(phi));
        }
    }

    tao::ld(a1,0);
    if( ls.fit(C2,CC.F,CC.G,a1,used,aerr) )
    {
        const size_t NMAX=100;
        const float   xc = a1[ *(*C2)["a"] ]/2;
        const float   yc = a1[ *(*C2)["b"] ]/2;
        const float   cc = a1[ *(*C2)["c"] ];
        const float   r  = sqrt( max_of<float>(0,cc+xc*xc+yc*yc) );
        ios::ocstream fp("c2fit.dat");
        for(size_t i=0;i<=NMAX;++i)
        {
            const float phi = (i*numeric<float>::two_pi)/NMAX;
            fp("%g %g\n", xc + r*cosf(phi), yc+r*sinf(phi));
        }
    }

    // automated
    ls.verbose = true;
    built_in::_circle<float> c,ce;
    if( CC.fit(C1,ls,c,ce) )
    {
        std::cerr << "c1 : (" << c.xc << "," << c.yc << ") r=" << c.r << std::endl;
        std::cerr << "err: (" << ce.xc << "," << ce.yc << ") r=" << ce.r << std::endl;
    }

    if( CC.fit(C2,ls,c,ce) )
    {
        std::cerr << "c2:  (" << c.xc << "," << c.yc << ") r=" << c.r << std::endl;
        std::cerr << "err: (" << ce.xc << "," << ce.yc << ") r=" << ce.r << std::endl;

    }

    return 0;
    
    // with the same radius, different centers

    *db << "C" << "x1" << "y1" << "x2" << "y2";

    (*C1).free();
    (*C2).free();

    (*C1)("a",(*db)("x1"));
    (*C1)("b",(*db)("y1"));
    (*C1)("c",(*db)("C"));


    (*C2)("a",(*db)("x2"));
    (*C2)("b",(*db)("y2"));
    (*C2)("c",(*db)("C"));

    std::cerr << "C1: " << *C1 << std::endl;
    std::cerr << "C2: " << *C2 << std::endl;

    vector<float> a5(5,0);
    vector<bool>  u5(5,true);

    db.setup(a5);

    const float dbD2 = db.D2(CC.F,a5);
    std::cerr << "dbD2=" << dbD2 << std::endl;

    matrix<float> alpha(5,5);
    vector<float> beta(5,0);
    const float   dbD2a = db.D2(alpha, beta, CC.F, CC.G, a5, u5);
    std::cerr << "dbD2a=" << dbD2a << std::endl;
    std::cerr << "beta  =" << beta  << std::endl;
    std::cerr << "alpha =" << alpha << std::endl;
    if(LU::build(alpha))
    {
        LU::solve(alpha,beta);
        std::cerr << "found=" << beta << std::endl;
        const double xa = beta[ *(*C1)["a"] ]/2;
        const double ya = beta[ *(*C1)["b"] ]/2;
        const double ca = beta[ *(*C1)["c"] ];
        const double xb = beta[ *(*C2)["a"] ]/2;
        const double yb = beta[ *(*C2)["b"] ]/2;
        const double cb = beta[ *(*C2)["c"] ];

        std::cerr << "xa=" << xa << ", ya=" << ya << std::endl;
        std::cerr << "xb=" << xb << ", yb=" << yb << std::endl;
        std::cerr << "ca=" << ca << ", cb=" << cb << std::endl;

        const double ra = sqrt( max_of<double>(0,ca+xa*xa+ya*ya) );
        const double rb = sqrt( max_of<double>(0,cb+xb*xb+yb*yb) );
        std::cerr << "ra=" << ra << ", rb=" << rb << std::endl;
    }







}
Y_UTEST_DONE()


