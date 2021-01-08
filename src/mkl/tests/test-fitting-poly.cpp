
#include "y/mkl/fitting/built-in/polynomial.hpp"
#include "y/mkl/fitting/sample.hpp"
#include "y/mkl/fitting/least-squares.hpp"
#include "y/mkl/fitting/sample/display.hpp"

#include "y/ios/ocstream.hpp"
#include "y/utest/run.hpp"


using namespace upsylon;
using namespace mkl;
using namespace fitting;

namespace
{

    template <typename T>
    void do_poly()
    {
        
        typedef sample<T,T> sample_type;
        const size_t          n = 10 + alea.leq(100);
        auto_ptr<sample_type> s = sample_type::create("poly",n);
        for(size_t i=n;i>0;--i)
        {
            const T x = T(2) * alea.to<T>();
            const T y = sin_of(x);
            s->add(x,y);
        }
        
        addressable<T> &X = s->abscissa;
        addressable<T> &Y = s->ordinate;
        
        hsort(X,Y,comparison::increasing<T>);
        
        built_in::polynomial<T> poly("a",5);
        vector<T>               aorg(poly.coeffs,0);
        vector<bool>            used(poly.coeffs,false);
        variables              &vars = **s;
        
        poly.make(vars);
        
        std::cerr << "poly: " << poly.vnames << std::endl;
        std::cerr << "vars: " << vars        << std::endl;
        
        aorg[1] = 0;
        aorg[2] = 1;
        
        {
            ios::ocstream fp("poly.dat");
            for(size_t i=1;i<=n;++i)
            {
                fp("%g %g %g\n", X[i], Y[i], poly.eval(X[i], aorg, vars) );
            }
            
        }
        
        
        
        
    }
    
}

Y_UTEST(fitting_poly)
{
    
    do_poly<float>();
    
}
Y_UTEST_DONE()

