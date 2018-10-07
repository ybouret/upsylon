#include "y/codec/asf.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"

using namespace upsylon;

Y_UTEST(asf)
{
    ASF::Alphabet alpha;
    alpha.display(std::cerr);

    {
        ios::icstream fp( ios::cstdin );
        string line;
        while( (std::cerr << "> ").flush(), fp.gets(line) )
        {
            for(size_t i=0;i<line.size();++i)
            {
                alpha.add(line[i]);
            }
            alpha.display(std::cerr);
        }

    }

}
Y_UTEST_DONE()

