
#include "y/mkl/fitting/built-in/polynomial.hpp"
#include "y/mkl/fitting/sample.hpp"
#include "y/mkl/fitting/least-squares.hpp"
#include "y/mkl/fitting/sample/display.hpp"

#include "y/ios/ocstream.hpp"
#include "y/utest/run.hpp"
#include "y/type/rtti.hpp"

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
        addressable<T> &Z = s->adjusted;

        hsort(X,Y,comparison::increasing<T>);
        
        built_in::polynomial<T> poly("a",4);
        vector<T>               aorg(poly.coeffs,0);
        vector<T>               aerr(poly.coeffs,0);
        vector<bool>            used(poly.coeffs,false);
        variables              &vars = **s;
        
        poly.make(vars);
        
        std::cerr << "poly: " << poly.names << std::endl;
        std::cerr << "vars: " << vars       << std::endl;
        
        
        
        least_squares<T,T> ls(true);
        const string fileName = "poly-" + rtti::name_of<T>() + ".dat";
        
        {
            ios::ocstream fp(fileName);
            for(size_t i=1;i<=n;++i)
            {
                fp("%g %g\n", X[i], Y[i]);
            }
            fp << "\n";
        }
        
        for(size_t i=1;i<=poly.coeffs;++i)
        {
            used[i] = true;
            if(i>1)
            {
                used[1] = false;
                aorg[1] = 0;
            }
            if( ls.fit(*s,poly,poly,aorg,used,aerr) )
            {
                display_variables::errors(std::cerr,NULL, vars, aorg, used, aerr);
                ios::ocstream fp(fileName,true);
                for(size_t i=1;i<=n;++i)
                {
                    fp("%g %g\n", X[i], Z[i]);
                }
                fp << "\n";
            }
        }
        display_sample::results(std::cerr,*s,aorg, used, aerr);

        
    }
    
}

Y_UTEST(fitting_poly)
{
    
    do_poly<float>();
    do_poly<double>();

}
Y_UTEST_DONE()

