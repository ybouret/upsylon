
#include "y/core/addr-list.hpp"
#include "y/core/addr-pool.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(core_anode)
{
    int          data[] = { 0, 1, 2, 4, 5 };
    const size_t ndat   = sizeof(data)/sizeof(data[0]);

    core::addr_list<int> alist;
    core::addr_pool<int> apool;
    const size_t         count = 10 + alea.leq(10);

    for(size_t i=count;i>0;--i)
    {
        if( alea.choice() )
        {
            alist.add( &data[alea.lt(ndat)] );
        }
        else
        {
            alist.pre( &data[alea.lt(ndat)] );
        }
        apool.push( &data[alea.lt(ndat)] );
    }
    Y_CHECK(alist.size==count);
    Y_CHECK(apool.size==count);

    {
        core::addr_node<int> *p = alist.head;
        core::addr_node<int> *q = apool.head;
        for(size_t i=count;i>0;--i)
        {
            std::cerr << **p << "/" << **q << " ";
            p=p->next;
            q=q->next;
        }
        std::cerr << std::endl;
    }

    while(alist.size)
    {
        if( alea.choice() )
        {
            delete alist.rem();
        }
        else
        {
            delete alist.pop();
        }
    }

    while(apool.size)
    {
        delete apool.pop();
    }

}
Y_UTEST_DONE()

