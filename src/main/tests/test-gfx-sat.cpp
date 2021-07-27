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
    const size_t total = 255*256/2;
    size_t       count = 0;
    uint8_t      sat[total];
    for(unsigned i=0;i<255;++i)
    {
        for(unsigned j=0;j<=i;++j)
        {
            sat[count] = 0;
            if(i>0)
            {
                sat[count] = static_cast<uint8_t>(j*255/i);
            }
            const size_t sub = i*(i+1)/2+j;
            Y_ASSERT(sub==count);
            ++count;

        }
    }
    std::cerr << "count=" << count << std::endl;
    Y_CHECK(total==count);



}
Y_UTEST_DONE()
 
