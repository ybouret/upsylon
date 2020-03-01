#include "y/type/bcopy.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace {
    
    
    
    template <typename T>
    void doBCopy()
    {
        static const size_t nmax = 1000;
        T source[nmax];
        T target[nmax];
        for(size_t n=0;n<=nmax;++n)
        {
            alea.fill(source,sizeof(source));
            memset(target,0,sizeof(target));
            bcopy(target,source,n);
            Y_ASSERT( 0 == memcmp(source, target, n*sizeof(T) ) );
        }
        
    }
    
}

Y_UTEST(bcopy)
{
    doBCopy<char>();
    doBCopy<short>();
    doBCopy<int32_t>();
    doBCopy<uint64_t>();

}
Y_UTEST_DONE()

