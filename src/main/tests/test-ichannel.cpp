#include "y/ios/ichannel.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

using namespace upsylon;

Y_UTEST(ichannel)
{
    for(size_t n=0;n<=1024;n += 1+ alea.leq(100) )
    {
        ios::bulk::pointer pBuff = new ios::bulk(n);
        std::cerr << n << " => " << pBuff->size << std::endl;
    }

    ios::bulk::pointer      buff = new ios::bulk( BUFSIZ );
    for(int i=1;i<argc;++i)
    {
        auto_ptr<ios::ichannel> ich = NULL;
        const string filename = argv[i];
        if("STDIN"==filename)
        {
            ich = new ios::ichannel( ios::cstdin, buff );
        }
        else
        {
            ich = new ios::ichannel(argv[i],buff);
        }
        string line;
        while( ich->gets(line) )
        {
            std::cerr << line << std::endl;
        }
    }
}
Y_UTEST_DONE()


