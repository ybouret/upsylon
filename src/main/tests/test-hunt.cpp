#include "y/code/hunt.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/type/rtti.hpp"

using namespace upsylon;


namespace
{
    template <typename T> static inline
    void test_hunt()
    {
        std::cerr << "hunting with " << rtti::name_of<T>() << std::endl;

        const size_t n = 2 + alea.lt(100);
        vector<T>    x(n,0);
        for(size_t i=2;i<=n;++i)
        {
            x[i] = x[i-1]+ alea.to<T>();
        }
        const T xmax = x[n];
        std::cerr << x[1] << " -> " << x[n] << std::endl;
        
        std::cerr << "\ttrack linear..." << std::endl;
        T *source = &x[1]-1;
        for(T fac=0;fac<=1;fac+=T(0.01))
        {
            const T xx = clamp<T>(0,fac*xmax,xmax);
            size_t  jlo = 1;
            size_t  jhi = n;
            size_t  j   = hunt::track(xx,source,jlo, jhi);
            Y_ASSERT(j>=jlo);
            Y_ASSERT(j<=jhi);
        }

#define ITER 100000
        std::cerr << "\ttrack random..." << std::endl;
        for(size_t iter=0;iter<ITER;++iter)
        {
            const T xx  = clamp<T>(0,alea.to<T>()*xmax,xmax);
            size_t  jlo = 1;
            size_t  jhi = n;
            size_t  j   = hunt::track(xx,source, jlo, jhi);
            Y_ASSERT(j>=jlo);
            Y_ASSERT(j<=jhi);
        }

        std::cerr << "\thunt generic..." << std::endl;
        const T ampli = T(0.6) * xmax;
        const T xhalf = xmax/2;
        size_t  j     = 0;
        for(size_t iter=0;iter<ITER;++iter)
        {
            const T            xx  = xhalf + alea.symm<T>() * ampli;
            const hunt::status res = hunt::search(xx,source,n,j);
            switch(res)
            {
                case hunt::found_below: Y_ASSERT(xx<0);    break;
                case hunt::found_above: Y_ASSERT(xx>xmax); break;
                case hunt::found_inner:
                    Y_ASSERT(j>=1);
                    Y_ASSERT(j<n);
                    Y_ASSERT(xx>=x[j]);
                    Y_ASSERT(xx<=x[j+1]);
                    break;
            }
            
            {
                size_t j2=0;
                Y_ASSERT(res==hunt::search(xx,x,j2));
                Y_ASSERT(j2==j);
            }

            {
                size_t j3=0;
                Y_ASSERT(res==hunt::lookup(xx,x,j3));
                Y_ASSERT(j3==j);
            }
        }
        std::cerr << std::endl;

      
        
    }
}


Y_UTEST(hunt)
{
    test_hunt<float>();
    test_hunt<double>();

}
Y_UTEST_DONE()

