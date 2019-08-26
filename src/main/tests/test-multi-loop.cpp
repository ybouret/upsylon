#include "y/code/multi-loop.hpp"
#include "y/utest/run.hpp"

#include "y/type/point3d.hpp"

using namespace upsylon;

namespace
{

    template <typename T>
    static inline
    void do_print( core::multi_loop<T> &L )
    {
        typename core::multi_loop<T>::const_type *value = L.current();
        std::cerr << '[' << value[0];
        for(size_t i=1;i<L.dimensions;++i)
        {
            std::cerr << '|' << value[i];
        }
        std::cerr << ']';

        std::cerr << std::endl;
    }

    template <typename T>
    static inline
    void do_loop( core::multi_loop<T> &L )
    {
        L.start();
        do_print(L);
        while(L.index<L.count)
        {
            L.next();
            do_print(L);
        }
    }
    

}


Y_UTEST(mloop)
{
    {
        unit_t lo = 1;
        unit_t up = 4;
        unit_t cr = 0;

        {
            core::multi_loop<unit_t> L(1,&lo,&up,&cr);
            std::cerr << "#count=" << L.count << std::endl;
            do_loop(L);
        }

        {
            core::multi_loop<unit_t> L(1,&lo,&up,&cr);
            std::cerr << "#count=" << L.count << std::endl;
            do_loop(L);
        }


        for(size_t iter=0;iter<100;++iter)
        {
            lo = alea.range<unit_t>(-10,10);
            up = alea.range<unit_t>(-10,10);
            if(lo>up) cswap(lo,up);
            core::multi_loop<unit_t> L(1,&lo,&up,&cr);
            do_loop(L);
        }

    }

    {
        point2d<int> lo(1,1);
        point2d<int> up(5,3);
        point2d<int>       cr;

        {
            core::multi_loop<int> L(2,&lo.x,&up.x,&cr.x);
            std::cerr << "#count=" << L.count << std::endl;
            do_loop(L);
        }


    }

    {
        const point3d<size_t>    lo(1,1,1);
        const point3d<size_t>    up(2,3,4);
        point3d<size_t>          cr;
        core::multi_loop<size_t> L(3,&lo.x,&up.x,&cr.x);
        std::cerr << "#count=" << L.count << std::endl;

        do_loop(L);

    }

}
Y_UTEST_DONE()


