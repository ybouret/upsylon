#include "y/core/locate.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/sort/unique.hpp"

using namespace upsylon;

Y_UTEST(locate)
{
    vector<int> data(1024,as_capacity);
    for(size_t iter=0;iter<10;++iter)
    {
        data.free();
        for(size_t i = alea.leq(30);i>0;--i)
        {
            data.push_back( 50 - int(alea.leq(100)));
        }

        unique(data);
        std::cerr << "data=" << data << std::endl;
        const size_t n = data.size();
        for(size_t i=1;i<=n;++i)
        {
            size_t  idx = 0;
            int    *ptr = core::locate( data[i], *data, n, comparison::increasing<int>, idx );
            Y_ASSERT(ptr==&data[i]);
            Y_ASSERT(idx==i-1);
        }
    }
}
Y_UTEST_DONE()

