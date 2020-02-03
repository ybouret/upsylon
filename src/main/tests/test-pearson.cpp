#include "y/hashing/pearson.hpp"
#include "y/string.hpp"
#include "y/sequence/vector.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"

using namespace upsylon;

namespace {

    void testH( hashing::pearson &p, const accessible<string> &arr )
    {
        uint32_t ini = 0;
        p.initialize(ini);
        std::cerr << "ini=" << std::hex << ini << std::dec << std::endl;
    }
}

Y_UTEST(pearson)
{
    hashing::pearson p;
    vector<string>   strings;
    if(argc>1)
    {
        ios::icstream fp( argv[1] );
        string line;
        while( fp.gets(line) )
        {
            strings << line;
        }
    }

    testH(p,strings);

    for(size_t iter=0;iter<4;++iter)
    {
        p.reset( &alea );
        testH(p,strings);
    }



}
Y_UTEST_DONE()

