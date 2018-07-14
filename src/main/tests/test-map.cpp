#include "y/associative/map.hpp"
#include "y/utest/run.hpp"


using namespace upsylon;

Y_UTEST(map)
{
    map<size_t,int> m;
    for(size_t i=0;i<=1000;++i)
    {
        Y_ASSERT(m.insert(i, alea.full<int>() ));
    }
    Y_ASSERT(m.size()==1001);

}
Y_UTEST_DONE()

