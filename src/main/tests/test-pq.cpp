#include "y/ordered/priority-queue.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/comparator.hpp"
#include "support.hpp"
#include "y/type/spec.hpp"

using namespace upsylon;

namespace {

    template <typename T>
    static inline void doPQ()
    {
        std::cerr << std::endl << type_name_of<T>() << std::endl;
        const size_t n = 1+alea.leq(20);
        vector<T>    data(n,as_capacity);
        vector<T*>   meta(n,0);

        increasing_comparator<T> compare;
        size_t  size = 0;
        T     **slot = *meta;
        for(size_t i=1;i<=n;++i)
        {
            const T tmp = support::get<T>();
            data.push_back_(tmp);
            core::prio_queue<T>::enqueue( &data[i], slot, size, compare);
        }


        vector<T*> meta2(n,0);
        core::priority_queue<T,increasing_comparator<T> > pq( *meta2, n );
        for(size_t i=1;i<=n;++i)
        {
            pq.enqueue( &data[i] );
        }
        std::cerr << "data=" << data << std::endl;
        while(size>0)
        {
            const T &lhs = * core::prio_queue<T>::extract(slot, size, compare);
            const T &rhs = *pq.extract();
            std::cerr << "  " <<  lhs << " / " << rhs << std::endl;
            Y_CHECK(lhs==rhs);
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


