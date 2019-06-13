#include "y/math/signal/curve.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace math;


template <typename T>
static inline void show_info()
{
    std::cerr << "sizeof=" << sizeof(T) << std::endl;
    std::cerr << "\ttype1d: " <<  curve::info_for<T>::dim << std::endl;
    std::cerr << "\ttype2d: " <<  curve::info_for< point2d<T> >::dim << std::endl;
    std::cerr << "\ttype2c: " <<  curve::info_for< complex<T> >::dim << std::endl;
    std::cerr << "\ttype3d: " <<  curve::info_for< point3d<T> >::dim << std::endl;
    Y_ASSERT(sizeof(T)==sizeof(typename curve::info_for<T>::real));
}

template <typename CURVE>
static inline void handle_curve( CURVE &C, const string &fn )
{
    std::cerr << "#points=" << C.size() << std::endl;
    {
        ios::ocstream fp(fn);
        C.save(fp);
    }

    typedef typename CURVE::point point;
    typedef typename CURVE::real  real;

    {
        curve::standard_spline<point> sspl;

        sspl.compute(C);
        {
            const string  out = "std-ns-" + fn;
            ios::ocstream fp(out);
            for(real t=0;t<=1.0;t+=0.01)
            {
                const point P = sspl.compute(t,C);
                const real  I = sspl.t2i(t,C.size());
                fp("%.15g", I);
                C.save_point(fp,&P);
                fp << '\n';
            }
        }
        sspl.lower_natural = sspl.upper_natural = false;
        sspl.compute(C);
        {
            const string  out = "std-zd-" + fn;
            ios::ocstream fp(out);
            for(real t=0;t<=1.0;t+=0.01)
            {
                const point P = sspl.compute(t,C);
                const real  I = sspl.t2i(t,C.size());
                fp("%.15g", I);
                C.save_point(fp,&P);
                fp << '\n';
            }
        }
    }

    {
        curve::periodic_spline<point> pspl;
        
    }


    
}

Y_UTEST(curve)
{
    show_info<float>();
    show_info<double>();
    
    curve::points< float >          C1f;
    curve::points< point2d<float> > C2f;
    curve::points< complex<float> > C2cf;
    curve::points< point3d<float> > C3f;
    
    curve::points< double >          C1d;
    curve::points< point2d<double> > C2d;
    curve::points< complex<float> >  C2cd;
    curve::points< point3d<double> > C3d;
    
    for(size_t i=3+alea.leq(10);i>0;--i)
    {
        {
            const float x = alea.symm<float>();
            const float y = alea.symm<float>();
            const float z = alea.symm<float>();
            
            C1f.add(x);
            C2f.add(x,y);
            C2cf.add(x,y);
            C3f.add(x,y,z);
            
            C1d.add(x);
            C2d.add(x,y);
            C2cd.add(x,y);
            C3d.add(x,y,z);
        }
        
    }
    
    handle_curve(C1f,"c1f.dat");
    handle_curve(C2f,"c2f.dat");
    handle_curve(C2cf,"c2cf.dat");
    handle_curve(C3f,"c3f.dat");
    
    handle_curve(C1d,"c1d.dat");
    handle_curve(C2d,"c2d.dat");
    handle_curve(C2cd,"c2cd.dat");
    handle_curve(C3d,"c3d.dat");
    
    
    
}
Y_UTEST_DONE()

