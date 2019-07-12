
#include "y/ios/icstream.hpp"
#include "y/ios/imstream.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"

using namespace upsylon;

Y_UTEST(istreams)
{

    {
        const string  data = "Hello, World";
        ios::imstream fp(data);
        char C=0;
        while( fp.query(C) )
        {
            fp.store(C);
            Y_ASSERT(fp.query(C));
            std::cerr << C;
        }
        std::cerr << std::endl;
    }

    if(argc>1 && 0==strcmp(argv[1],"true"))
    {
        ios::icstream fp( ios::cstdin );
        string line;
        while( fp.gets(line) )
        {
            std::cerr << line << std::endl;
        }
    }


}
Y_UTEST_DONE()
