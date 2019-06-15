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

    const real tn = real(C.size());
    {
        curve::standard_spline<point> F;
        {
            const string  out = "std-n-" + fn;
            ios::ocstream fp(out);
            F.compute(C);
            for(real t=1;t<=tn;t+=0.02)
            {
                point p = F.compute(t,C);
                fp("%g ",t);
                C.save_point(fp,&p) << '\n';
            }
        }
        F.lower_natural = F.upper_natural = false;
        {
            const string  out = "std-z-" + fn;
            ios::ocstream fp(out);
            F.compute(C);
            for(real t=1;t<=tn;t+=0.02)
            {
                point p = F.compute(t,C);
                fp("%g ",t);
                C.save_point(fp,&p) << '\n';
            }
        }
    }

    {
        curve::periodic_spline<point> F;
        const real tm = tn+1;
        {
            const string  out = "std-p-" + fn;
            ios::ocstream fp(out);
            F.compute(C);
            for(real t=1;t<=tm;t+=0.02)
            {
                point p = F.compute(t,C);
                fp("%g ",t);
                C.save_point(fp,&p) << '\n';
            }
        }
    }

}

#if 0
template <typename CURVE>
void metrics_for( CURVE &C, const string &fn )
{
    typedef typename CURVE::point point;
    typedef typename CURVE::real  real;

    {
        ios::ocstream fp(fn);
        for(size_t i=1;i<=C.size();++i)
        {
            C.save_point(fp, &C[i]) << '\n';
        }
    }

    const real tn = real(C.size());
    {
        curve::standard_spline<point> F;
        {
            const string  out = "std-n-" + fn;
            ios::ocstream fp(out);
            F.compute(C);
            for(real t=1;t<=tn;t+=0.02)
            {
                point p = F.compute(t,C);
                fp("%g ",t);
                C.save_point(fp,&p) << '\n';
            }
        }
        F.lower_natural = F.upper_natural = false;
        {
            const string  out = "std-z-" + fn;
            ios::ocstream fp(out);
            F.compute(C);
            for(real t=1;t<=tn;t+=0.02)
            {
                point p = F.compute(t,C);
                fp("%g ",t);
                C.save_point(fp,&p) << '\n';
            }
        }
    }

    {
        curve::periodic_spline<point> F;
        const real tm = tn+1;
        {
            const string  out = "std-p-" + fn;
            ios::ocstream fp(out);
            F.compute(C);
            for(real t=1;t<=tm;t+=0.02)
            {
                point p = F.compute(t,C);
                fp("%g ",t);
                C.save_point(fp,&p) << '\n';
            }
        }
    }


}
#endif

#include "y/string/convert.hpp"

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

    size_t np = 3;
    if(argc>1)
    {
        np = string_convert::to<size_t>(argv[1],"np");
    }

    const float  dz    = 1.0/np;
    const float  noise = 0.01f;
    for(size_t i=0;i<np;++i)
    {

        {
            const float theta  = (i*numeric<float>::two_pi)/np*(1.0f+noise*alea.symm<float>());
            const float radius = 1.0f+noise*alea.symm<float>();
            const float x = radius * cosf(theta);
            const float y = radius * sinf(theta);
            const float z = (dz * i) * (1.0f+noise*alea.symm<float>());
            
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
    
    handle_curve(C1f, "c1f.dat");
    handle_curve(C2f, "c2f.dat");
    handle_curve(C2cf,"c2cf.dat");
    handle_curve(C3f, "c3f.dat");
    
    handle_curve(C1d, "c1d.dat");
    handle_curve(C2d, "c2d.dat");
    handle_curve(C2cd,"c2cd.dat");
    handle_curve(C3d, "c3d.dat");

    //metrics_for(C2f, "m2f.dat");

    
}
Y_UTEST_DONE()

