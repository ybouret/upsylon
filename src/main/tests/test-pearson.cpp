#include "y/hashing/pearson.hpp"
#include "y/string.hpp"
#include "y/sequence/vector.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"

using namespace upsylon;

namespace {

    void testH( hashing::function &p, const accessible<string> &arr )
    {

        for(size_t i=1;i<=arr.size();++i)
        {
            const string   &s = arr[i];
            const uint64_t  h = p.key<uint64_t>(s);
            std::cerr << s << " : " << std::hex << h << std::dec << std::endl;
        }
    }
}

Y_UTEST(pearson)
{
    hashing::pearson<uint32_t> p;
    vector<string>            strings;
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

    for(size_t iter=0;iter<1;++iter)
    {
        p.reset( &alea );
        testH(p,strings);
    }



}
Y_UTEST_DONE()

