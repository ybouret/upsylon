#include "y/code/hunt.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;


namespace
{
    template <typename T> static inline
    void test_hunt()
    {
        const size_t n = 2 + alea.lt(100);
        vector<T>    x(n,0);
        for(size_t i=2;i<=n;++i)
        {
            x[i] = x[i-1]+ alea.to<T>();
        }
        const T xmax = x[n];
        std::cerr << x[1] << " -> " << x[n] << std::endl;
        
        std::cerr << "linear..." << std::endl;
        for(T fac=0;fac<=1;fac+=T(0.01))
        {
            const T xx = clamp<T>(0,fac*xmax,xmax);
            size_t  jlo = 1;
            size_t  jhi = n;
            size_t  j   = hunt::track(xx, &x[1]-1, jlo, jhi);
            Y_ASSERT(j>=jlo);
            Y_ASSERT(j<=jhi);
        }
       
        std::cerr << "random..." << std::endl;
        for(size_t iter=0;iter<10000;++iter)
        {
            const T xx  = clamp<T>(0,alea.to<T>()*xmax,xmax);
            size_t  jlo = 1;
            size_t  jhi = n;
            size_t  j   = hunt::track(xx, &x[1]-1, jlo, jhi);
            Y_ASSERT(j>=jlo);
            Y_ASSERT(j<=jhi);
        }



      
        
    }
}


Y_UTEST(hunt)
{
    test_hunt<float>();
    test_hunt<double>();

}
Y_UTEST_DONE()

