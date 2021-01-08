
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
            const T x = numeric<T>::two_pi * alea.to<T>();
            const T y = sin_of(x);
            s->add(x,y);
        }
        
        hsort(s->abscissa,s->ordinate,comparison::increasing<T>);
        
        built_in::polynomial<T> poly("a",5);
        vector<T>               aorg(poly.coeffs,0);
        vector<bool>            used(poly.coeffs,false);
        
        poly.make(**s);
        
        std::cerr << "poly: " << poly.vnames << std::endl;
        std::cerr << "vars: " << **s         << std::endl;

        
        
        
        
    }
    
}

Y_UTEST(fitting_poly)
{
    
    do_poly<float>();
    
}
Y_UTEST_DONE()

