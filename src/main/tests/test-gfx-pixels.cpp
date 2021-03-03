

#include "y/graphic/pblock.hpp"
#include "y/graphic/color/rgb.hpp"
#include "y/utest/run.hpp"
#include "y/type/spec.hpp"

using namespace upsylon;
using namespace graphic;

namespace
{
    static inline
    void fillnz(void *addr, size_t n) throw()
    {
        uint8_t *p = (uint8_t *)addr;
        while(n-- > 0)
        {
            uint8_t a = 0;
            while(a<=0) a = alea.full<uint8_t>();
            *(p++) = a;
        }
    }

    template <typename T> static inline
    void check_block( pblock<T> &p )
    {
        std::cerr << "\tsize=" << p.size << ", maxi=" << p.maxi << ", block_size=" << p.block_size << std::endl;
        for(size_t i=0;i<p.size;++i)
        {
            fillnz(&p[i],sizeof(T));
        }
    }

    template <typename T> static inline
    void check_blocks()
    {
        std::cerr << type_name_of<T>() << std::endl;
        for(size_t i=0; i <= 1000; i += alea.leq(100) )
        {
            pblock<T> iblk(i);
            check_block(iblk);
        }
    }

}

Y_UTEST(gfx_pixels)
{
    check_blocks<int>();
    check_blocks<char>();
    check_blocks<rgba>();

}
Y_UTEST_DONE()
