
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
        sorted_list<T,increasing_comparator<T> >                  il;
        sorted_vector<T,increasing_comparator<T>,memory::global>  ig;
        sorted_vector<T,increasing_comparator<T>,memory::pooled>  ip;

        sorted_list<T,decreasing_comparator<T> >                  dl;
        sorted_vector<T,decreasing_comparator<T>,memory::global>  dg;
        sorted_vector<T,decreasing_comparator<T>,memory::pooled>  dp;

    }

}

Y_UTEST(ordered)
{
    doOrdered<int>();
    
}
Y_UTEST_DONE()




