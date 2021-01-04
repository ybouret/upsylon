
#include "y/mkl/fitting/built-in/circle.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"

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
        for(size_t i=3+alea.leq(10);i>0;--i)
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
    
    fill(C1,12,-11,34); { ios::ocstream fp("c1.dat"); save_sample(C1,fp); }
    fill(C2,-2,7,34);   { ios::ocstream fp("c2.dat"); save_sample(C2,fp); }
    circle_type CC;

    *C1 << "a" << "b" << "c";
    *C2 << "c" << "a" << "b";

    vector<float> a1(3,0);
    vector<float> a2(3,0);

    C1.setup(a1);
    C2.setup(a2);
    const float C1D2 = C1.D2(CC.func,a1); std::cerr << "C1.D2=" << C1D2 << std::endl;
    const float C2D2 = C2.D2(CC.func,a2); std::cerr << "C2.D2=" << C2D2 << std::endl;


    
    
    
}
Y_UTEST_DONE()


