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

        {
            const string  out = "std-ns-v-" + fn;
            ios::ocstream fp(out);
            for(real t=0;t<=1.0;t+=0.01)
            {
                const point P = sspl.tangent(t,C,NULL);
                const real  I = sspl.t2i(t,C.size());
                fp("%.15g", I);
                C.save_point(fp,&P);
                fp(" %.15g", sqrt_of( curve::norm2(P) ) );
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

        {
            const string  out = "std-zd-v-" + fn;
            ios::ocstream fp(out);
            for(real t=0;t<=1.0;t+=0.01)
            {
                const point P = sspl.tangent(t,C,NULL);
                const real  I = sspl.t2i(t,C.size());
                fp("%.15g", I);
                C.save_point(fp,&P);
                fp(" %.15g", sqrt_of(curve::norm2(P)));
                fp << '\n';
            }
        }
    }

    {
        curve::periodic_spline<point> pspl;
        pspl.compute(C);
        {
            const string  out = "pbc-" + fn;
            ios::ocstream fp(out);
            for(real t=-1.0;t<=2.0;t+=0.01)
            {
                const point P = pspl.compute(t,C);
                const real  I = pspl.t2i(t,C.size());
                fp("%.15g", I);
                C.save_point(fp,&P) << '\n';
            }
        }
    }


}


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

    {
        curve::standard_spline<point> spl;
        spl.compute(C);
        {
            const string  out = "std-v-" + fn;
            ios::ocstream fp(out);
            for(real t=0;t<=1.0;t+=0.02)
            {
                point s;
                point p = spl.compute(t,C,&s);
                C.save_point(fp,&p) << '\n';
                p += s/(4*C.size());
                C.save_point(fp,&p) << '\n';
                fp << '\n';
            }
        }

        {
            const string  out = "std-a-" + fn;
            ios::ocstream fp(out);
            for(real t=0;t<=1.0;t+=0.02)
            {
                point a;
                point p = spl.compute(t,C,NULL,&a);
                C.save_point(fp,&p) << '\n';
                p += a;
                C.save_point(fp,&p) << '\n';
                fp << '\n';
            }
        }

        {
            const string out = "std-arc-" + fn;
            ios::ocstream fp(out);
            size_t np=1000;
            for(size_t i=0;i<=np;++i)
            {
                const real t = real(i)/np;
                const real s = spl.speed(t,C);
                const real l = spl.arc_length(0,t,C);
                const real g = spl.curvature(t,C);
                fp("%g %g %g %g\n", t,s,l,g);
            }
        }
    }

    {
        curve::periodic_spline<point> spl;
        spl.compute(C);

        {
            const string out = "pbc-" + fn;
            ios::ocstream fp(out);
            for(real t=0;t<=1.0;t+=0.01)
            {
                point p = spl.compute(t,C);
                C.save_point(fp,&p) << '\n';
            }
        }

        {
            const string  out = "pbc-v-" + fn;
            ios::ocstream fp(out);
            for(real t=0;t<=1.0;t+=0.02)
            {
                point s;
                point p = spl.compute(t,C,&s);
                C.save_point(fp,&p) << '\n';
                p += s/(4*C.size());
                C.save_point(fp,&p) << '\n';
                fp << '\n';
            }
        }

        {
            const string  out = "pbc-a-" + fn;
            ios::ocstream fp(out);
            for(real t=0;t<=1.0;t+=0.02)
            {
                point a;
                point p = spl.compute(t,C,NULL,&a);
                C.save_point(fp,&p) << '\n';
                p += a;
                C.save_point(fp,&p) << '\n';
                fp << '\n';
            }
        }

        {
            const string out = "pbc-arc-" + fn;
            ios::ocstream fp(out);
            size_t np=1000;
            for(size_t i=0;i<=np;++i)
            {
                const real t = real(i)/np;
                const real s = spl.speed(t,C);
                const real l = spl.arc_length(0,t,C);
                const real g = spl.curvature(t,C);
                fp("%g %g %g %g\n", t,s,l,g);
            }
        }
        std::cerr << "length = " << spl.arc_length(0, 1,C) << std::endl;
        std::cerr << "area   = " << spl.area(C)            << std::endl;
    }

}

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

    const float  dz = 1.0/np;
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
    
    metrics_for(C2f, "m2f.dat");

    
}
Y_UTEST_DONE()

