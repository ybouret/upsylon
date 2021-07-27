#include "y/utest/run.hpp"
#include "y/gfx/color/rgb.hpp"

#include "y/ios/ocstream.hpp"
#include "y/type/utils.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;
using namespace graphic;


Y_UTEST(gfx_sat)
{
    if(false)
    {
        ios::ocstream fp("saturated.inc");
        const unsigned total = 257*256/2;
        unsigned       count = 0;

        fp("const uint8_t saturated_table[%u] = {\n",total);
        for(unsigned i=0;i<256;++i)
        {
            for(unsigned j=0;j<=i;++j)
            {
                uint8_t sat = 0;
                if(i>0)
                {
                    sat = static_cast<uint8_t>((j*255)/i);
                }
                fp(" %3u",unsigned(sat));
                const unsigned sub = i*(i+1)/2+j;
                Y_ASSERT(sub==count);
                ++count;
                if(count<total)
                {
                    fp << ',';
                }
            }
            fp << '\n';
        }
        std::cerr << "count=" << count << std::endl;
        Y_CHECK(total==count);
        fp << "};\n";
    }

}
Y_UTEST_DONE()

