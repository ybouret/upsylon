
#include "y/ordered/sorted-list.hpp"
#include "y/ordered/sorted-vector.hpp"
#include "y/memory/pooled.hpp"

#include "y/utest/run.hpp"
#include <typeinfo>

#include "support.hpp"

using namespace upsylon;


namespace
{

    template <typename ORDERED>
    static inline void checkOrdered()
    {
        ordered_multiple<ORDERED> M;
        ordered_single<ORDERED>   S;
        ordered_unique<ORDERED>   U;

        std::cerr << "checkOrdered< " << typeid(ORDERED).name() << ">" << std::endl;

        typedef typename ORDERED::mutable_type type;

        for(size_t cycle=0;cycle<100;++cycle)
        {
            M.release();
            S.release();
            U.release();

            size_t       all_count = 0;
            size_t       one_count = 0;
            for(size_t iter=10+alea.leq(100);iter>0;--iter)
            {
                const type tmp = support::get<type>();
                for(size_t trials=1+alea.leq(2);trials>0;--trials)
                {
                    ++all_count;
                    M.insert(tmp);
                    S.insert(tmp);
                    if(U.insert(tmp))
                    {
                        ++one_count;
                    }
                }
            }
            Y_ASSERT(M.size()==all_count);
            Y_ASSERT(S.size()==one_count);
            Y_ASSERT(U.size()==one_count);
        }

    }

    template <typename T>
    static inline void doOrdered()
    {

        checkOrdered< sorted_list<T,increasing_comparator<T> > >();
        checkOrdered< sorted_list<T,decreasing_comparator<T> > >();

        checkOrdered< sorted_vector<T,increasing_comparator<T>,memory::global> >();
        checkOrdered< sorted_vector<T,decreasing_comparator<T>,memory::global> >();

        checkOrdered< sorted_vector<T,increasing_comparator<T>,memory::pooled> >();
        checkOrdered< sorted_vector<T,decreasing_comparator<T>,memory::pooled> >();



    }

}

Y_UTEST(ordered)
{
    doOrdered<short>();
    doOrdered<int>();
    doOrdered<double>();
    doOrdered<string>();

}
Y_UTEST_DONE()




