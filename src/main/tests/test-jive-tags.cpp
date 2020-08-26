

#include "y/jive/tags.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;
using namespace Jive;

Y_UTEST(jive_tags)
{
    {
        const Tag hello = Tags::Make( "hello" );
        std::cerr << "hello=" << hello << std::endl;
        const Tag world = Tags::Make( 'C' );
        std::cerr << "world=" << world << std::endl;
        {
            const string tmp = "tmp";
            const Tag    temp = Tags::Make(tmp);
            std::cerr << "temp=" << tmp << std::endl;
            const Tag    tdup = Tags::Make(temp);
            std::cerr << "tdup=" << tdup << std::endl;
        }
        const Tag dyn = Tags::Make( new string("dyn") );
        std::cerr << "dyn=" << dyn << std::endl;
    }

    Tags::Display();
}
Y_UTEST_DONE()

