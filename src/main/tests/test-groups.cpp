
#include "y/sort/groups.hpp"
#include "y/utest/run.hpp"

#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"
#include "support.hpp"
#include "y/sort/sorted-sum.hpp"

using namespace upsylon;

namespace
{
    template <typename T>
    static inline void doGroups()
    {
        vector<size_t> vgroups;
        vector<size_t> lgroups;
        
        vector<T> V;
        list<T>   L;
        for(size_t n=0;n<=8;++n)
        {
            std::cerr << "[" << n << "]" << std::endl;
            V.free();
            L.free();
            while(V.size()<n)
            {
                const T tmp = support::get<T>();
                for(size_t i=1+alea.leq( (n-V.size()) >> 1 );i>0;--i)
                {
                    V.push_back(tmp);
                    L.push_back(tmp);
                    Y_ASSERT(V.size()==L.size());
                    if(V.size()>=n) goto GROUPS;
                }
            }

        GROUPS:
            Y_ASSERT(n==V.size());
            Y_ASSERT(n==L.size());
            alea.shuffle(*V,n);
            std::cerr << V << std::endl;
            std::cerr << L << std::endl;

            find_groups::by_sorting_of(V,vgroups);
            std::cerr << V << " -> " << vgroups << std::endl;

            find_groups::by_sorting_of(L,lgroups);
            std::cerr << L << " -> " << lgroups << std::endl;
            Y_CHECK(sorted_sum(vgroups)==n);
            Y_CHECK(sorted_sum(lgroups)==n);
            std::cerr << std::endl;
        }

    }
}

Y_UTEST(groups)
{
    doGroups<unsigned short>();
    doGroups<double>();
    doGroups<apq>();
}
Y_UTEST_DONE()
