#include "y/ipso/layout.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(ipso)
{
    for(size_t iter=0;iter<10;++iter)
    {
        const unit_t x1 = ipso::coord1D( alea.leq(100) ) - 50;
        const unit_t x2 = ipso::coord1D( alea.leq(100) ) - 50;
        ipso::layout_of<ipso::coord1D> L1(x1,x2);
        const unit_t y1 = ipso::coord1D( alea.leq(100) ) - 50;
        const unit_t y2 = ipso::coord1D( alea.leq(100) ) - 50;
        ipso::layout_of<ipso::coord2D> L2( ipso::coord2D(x1,y1), ipso::coord2D(x2,y2) );
        const unit_t z1 = ipso::coord1D( alea.leq(100) ) - 50;
        const unit_t z2 = ipso::coord1D( alea.leq(100) ) - 50;
        ipso::layout_of<ipso::coord3D> L3( ipso::coord3D(x1,y1,z1), ipso::coord3D(x2,y2,z2) );

        std::cerr << "L1=" << L1 << std::endl;
        std::cerr << "L2=" << L2 << std::endl;
        std::cerr << "L3=" << L3 << std::endl;


        {
            ipso::coord1D j=0;
            for(ipso::coord1D i=L1.lower;i<=L1.upper;++i,++j)
            {
                const ipso::coord1D idx = L1.index_of(i);
                Y_ASSERT(j==idx);
            }
        }

        {
            ipso::coord1D count=0;
            for(ipso::coord1D j=L2.lower.y;j<=L2.upper.y;++j)
            {
                for(ipso::coord1D i=L2.lower.x;i<=L2.upper.x;++i)
                {
                    const ipso::coord2D q(i,j);
                    const ipso::coord1D idx = L2.index_of(q);
                    Y_ASSERT(count==idx);
                    ++count;
                }
            }
        }

        {
            ipso::coord1D count=0;
            for(ipso::coord1D k=L3.lower.z;k<=L3.upper.z;++k)
            {
                for(ipso::coord1D j=L3.lower.y;j<=L3.upper.y;++j)
                {
                    for(ipso::coord1D i=L3.lower.x;i<=L3.upper.x;++i)
                    {
                        const ipso::coord3D q(i,j,k);
                        const ipso::coord1D idx = L3.index_of(q);
                        Y_ASSERT(count==idx);
                        ++count;
                    }
                }
            }
        }


    }

}
Y_UTEST_DONE()


