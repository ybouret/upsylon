
#include "y/ordered/sorted-list.hpp"
#include "y/ordered/sorted-vector.hpp"
#include "y/memory/pooled.hpp"

#include "y/utest/run.hpp"

using namespace upsylon;


namespace
{

    template <typename T>
    static inline void doOrdered()
    {
        sorted_list<T>                   l;
        sorted_vector<T,memory::global>  g;
        sorted_vector<T,memory::pooled>  p;

    }

}

Y_UTEST(ordered)
{
    doOrdered<int>();
    
}
Y_UTEST_DONE()




