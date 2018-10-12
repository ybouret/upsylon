#include "y/core/locate.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/sort/unique.hpp"

using namespace upsylon;

namespace
{
    static inline int new_data()
    {
        return 50 - int(alea.leq(100));
    }
}
Y_UTEST(locate)
{
    vector<int> data(1024,as_capacity);
    for(size_t iter=0;iter<1024;++iter)
    {
        std::cerr << std::endl;
        data.free();
        for(size_t i = alea.leq(30);i>0;--i)
        {
            data.push_back(new_data());
        }

        unique(data);
        std::cerr << "data=" << data << std::endl;

        // check each item is at its place
        for(size_t i=1;i<=data.size();++i)
        {
            size_t  idx = 0;
            int    *ptr = core::locate( data[i], *data, data.size(), comparison::increasing<int>, idx );
            Y_ASSERT(ptr==&data[i]);
            Y_ASSERT(idx==i-1);
        }

        // then find where to insert some items
        for(size_t i=alea.leq(10)+1;i>0;--i)
        {
            const int lhs = new_data();
            size_t    idx = 0;
            std::cerr << "Looking for " << lhs << " : ";
            int      *ptr = core::locate( lhs, *data, data.size(), comparison::increasing<int>, idx );
            ++idx;
            if(ptr)
            {
                Y_ASSERT(lhs==data[idx]);
                std::cerr << "Found@" << idx << std::endl;
            }
            else
            {
                std::cerr << "Not Found, index=" << idx << std::endl;
            }
        }

    }
}
Y_UTEST_DONE()

