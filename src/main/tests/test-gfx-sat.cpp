#include "y/utest/run.hpp"
#include "y/gfx/color/rgb.hpp"

#include "y/ios/ocstream.hpp"
#include "y/type/utils.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;
using namespace graphic;

namespace
{
}

Y_UTEST(gfx_sat)
{

    const size_t total = 256*257/2;
    size_t       count = 0;
    for(int i=0;i<=255;++i)
    {
        for(int j=0;j<=i;++j)
        {
            int ans = 0;
            if(i>0)
            {
                ans = (j*255)/i;
                Y_ASSERT(ans<=255);
            }
            ++count;
            fprintf(stderr," %3d",ans);
            if(count<total) fputc(',',stderr);
        }
        fputc('\n',stderr);
    }

    std::cerr << "count="<< count << std::endl;
    Y_CHECK(total==count);

}
Y_UTEST_DONE()
 
