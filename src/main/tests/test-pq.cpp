#include "y/ordered/priority-queue.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/comparator.hpp"
#include "support.hpp"

using namespace upsylon;

namespace {

    template <typename T>
    static inline void doPQ()
    {
        
        const size_t n = 1+alea.leq(100);
        vector<T>    data(n,as_capacity);
        vector<T*>   meta(n,0);
        increasing_comparator<T> compare;

        T     **slot = *meta;
        size_t  size=0;
        for(size_t i=1;i<=n;++i)
        {
            const T tmp = support::get<T>();
            data.push_back_(tmp);
            core::priority_queue<T>::enqueue( &data[i], slot, size, compare);
        }
        std::cerr << "data=" << data << std::endl;
        while(size>0)
        {
            std::cerr << "  " << * core::priority_queue<T>::extract(slot, size, compare) << std::endl;
        }
    }

}

Y_UTEST(pq)
{

    doPQ<int>();
    doPQ<double>();
    doPQ<mpn>();
    doPQ<mpz>();
    doPQ<mpq>();
    doPQ<string>();

}
Y_UTEST_DONE()


