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

    template <typename T>
    static inline void check_sorted(const array<T> &data )
    {
        for(size_t i=1;i<data.size();++i)
        {
            if(!(data[i]<data[i+1]))
            {
                std::cerr << "data[" << i << "]=" << data[i] << ", data[" << i+1 << "]=" << data[i+1] << std::endl;
                throw exception("not sorted ");
            }
        }
    }
}
Y_UTEST(locate)
{
    vector<int> data;
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
        for(size_t i=alea.leq(50)+1;i>0;--i)
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
                data.insert_at(idx,lhs);
                std::cerr << "data=" << data << std::endl;
                check_sorted(data);
            }
        }

        data.free();
        int collisions = 0;
        for(size_t i=alea.leq(50)+1;i>0;--i)
        {
            const int lhs = new_data();
            if(!core::insert_sorted(lhs,data,comparison::increasing<int>))
            {
                ++collisions;
            }
            else
            {
                std::cerr << "srt=" << data << std::endl;
            }
        }
        std::cerr << "#collisions=" << collisions << std::endl;

    }
}
Y_UTEST_DONE()

