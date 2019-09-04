#include "y/counting/dancing.hpp"
#include "y/utest/run.hpp"
#include "y/string/convert.hpp"

using namespace upsylon;

namespace  {
    static inline void do_all_dance(const size_t n)
    {
        std::cerr << "testing all dancing parties with n=" << n << std::endl;
        for(size_t k=1; k<=n; ++k)
        {
            dancing dance(n,k);
            for(size_t wg = dance.wg_max; wg>=dance.wg_min; --wg )
            {
                const dancing::frame *ini =0, *end=0;
                const size_t num = dance.find(wg, ini, end);
                if(num)
                {
                    Y_ASSERT(ini);
                    Y_ASSERT(end);
                    while(ini!=end->next)
                    {
                        Y_ASSERT(ini->workgroups==wg);
                        ini=ini->next;
                    }
                }
                else
                {
                    Y_ASSERT(!ini);
                    Y_ASSERT(!end);
                }

            }
        }
    }
}

Y_UTEST(dancing)
{
    size_t n=7; if(argc>1) n= string_convert::to<size_t>(argv[1],"n");
    size_t k=2; if(argc>2) k= string_convert::to<size_t>(argv[2],"k");
    dancing dance(n,k);
    
    for( const dancing::frame *f = dance.frames.head; f; f=f->next)
    {
        std::cerr << *f << std::endl;
    }

    std::cerr << "#amount=" << dance.amount      << std::endl;
    std::cerr << "#cycles=" << dance.frames.size << std::endl;

    for(size_t wg = dance.wg_max; wg>=dance.wg_min; --wg )
    {
        const dancing::frame *ini =0, *end=0;
        const size_t num = dance.find(wg, ini, end);
        std::cerr << "#workgroups=" << wg << " : " << num << std::endl;
    }

    for(size_t i=1;i<=15;++i)
    {
        do_all_dance(i);
    }



}
Y_UTEST_DONE()

