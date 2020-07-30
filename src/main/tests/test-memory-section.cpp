#include "y/memory/section.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/memory/cblock.hpp"

using namespace upsylon;

Y_UTEST(section)
{
    Y_UTEST_SIZEOF(memory::section::block);
    Y_UTEST_SIZEOF(memory::section);

    memory::cblock_of<size_t> buffer(memory::section::min_size_t + alea.leq(100) );
    std::cerr << "#bytes=" << buffer.bytes << std::endl;
    memory::section s(buffer.data, buffer.bytes);

    s.display();

    {
        size_t zero=0;
        Y_CHECK(s.acquire(zero)==NULL);
        Y_CHECK(zero==0);
    }

    {
        for(size_t i=1;i<=buffer.bytes;++i)
        {
            size_t n=i;
            void  *p = s.acquire(n);
            if(!p)
            {
            }
            else
            {
                std::cerr << "=>n=" << n << " @" << p << std::endl;
            }
        }
    }


}
Y_UTEST_DONE()

