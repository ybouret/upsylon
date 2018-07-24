#include "y/sort/nwsrt.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;

namespace
{
    template <typename T>
    static inline void do_test()
    {
        
        vector<T> v( nwsrt::max_size, as_capacity);
        for(size_t n=0;n<=nwsrt::max_size;++n)
        {
            for(size_t iter=0;iter<16;++iter)
            {
                v.free();
                for(size_t j=0;j<n;++j)
                {
                    const T x = support::get<T>();
                    v.push_back(x);
                }
                
                for(size_t jter=0;jter<16;++jter)
                {
                    alea.shuffle(*v,n);
                    nwsrt::on(*v,n);
                    for(size_t j=1;j<n;++j)
                    {
                        Y_ASSERT(v[j]<=v[j+1]);
                    }
                }
            }
        }
        
        
    }
}

Y_UTEST(nwsrt)
{
    do_test<int>();
    do_test<float>();
    do_test<string>();
}
Y_UTEST_DONE()

