#include "y/math/adjust/samples.hpp"
#include "y/math/adjust/variables.hpp"

#include "y/utest/run.hpp"
#include "y/memory/pooled.hpp"
#include "y/sequence/list.hpp"
#include "support.hpp"

using namespace upsylon;
using namespace Adjust;

namespace {
    
    template <typename T>
    Sample<T> *createSample()
    {
        const size_t n = 1 + alea.leq(10);
        typename Type<T>::Series X = new vector<T,memory::global>(n);
        typename Type<T>::Series Y = new vector<T,memory::pooled>(n);
        typename Type<T>::Series Z = new list<T>(n);
        
        support::fill1D(X);
        for(size_t i=n;i>0;--i)
        {
            Y[i] = X[i]*X[i];
        }
        
        return new Sample<T>(X,Y,Z);
    }
    
}

Y_UTEST(adjust)
{
    
    Samples<float>          S;
    Sample<float>::Pointer  S1 = createSample<float>();
    Sample<float>::Pointer  S2 = createSample<float>();
    S << S1 << S2;
    Y_CHECK(S.count()==S1->count() + S2->count() );
    
    
    Variables vars;
    vars << "t0" << "slope1" << "slope2";
    
    Variables vars1; vars1( vars["t0"] )( vars["slope1"], "slope");
    Variables vars2; vars2( vars["t0"] )( vars["slope2"], "slope");

    
    std::cerr << "vars  = " << vars << std::endl;
    std::cerr << "vars1 = " << vars1 << std::endl;
    std::cerr << "vars2 = " << vars2 << std::endl;

}
Y_UTEST_DONE()

