#include "y/counting/dancing.hpp"
#include "y/counting/comb.hpp"
#include "y/ptr/auto.hpp"

#include <iostream>
namespace upsylon
{
    dancing:: ~dancing() throw()
    {

    }

    static inline
    dancing::group * new_group_from( const combination &comb )
    {
        auto_ptr<dancing::group> grp = new dancing::group();
        for(size_t i=comb.k;i>0;--i)
        {
            grp->push_front( new dancing::guest( comb[i] ) );
        }
        return grp.yield();
    }

    dancing:: dancing(const size_t n,
                      const size_t k)
    {
        combination  comb(n,k);
        const size_t groups = n/k;
        const size_t alones = n-k*groups;
        std::cerr << "dancing(" << n << "," << k << ") : #groups=" << groups << ", #alones=" << alones << std::endl;

        for( comb.start(); comb.valid(); comb.next() )
        {
            std::cerr << "got: " << comb << std::endl;
            auto_ptr<group> grp = new_group_from(comb);
        }


    }


}
