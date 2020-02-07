#include "y/string/esma.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"

using namespace upsylon;

Y_UTEST(esma)
{

    {
        core::esma<char> m( "abc", 3 );
        m.find("abcdabcd",8);
    }

    {
        core::esma<char> m( "abcd" );

    }

    return 0;

    for(size_t iter=0;iter<1024;++iter)
    {
        string tmp = support::get<string>();
        const core::esma<char> kmp( tmp );
        
    }



#if 0
    { core::esma mgr; }

    {
        core::esma mgr;

        mgr.prepare(0, 1);
        for(size_t iter=0;iter<1024;++iter)
        {
            const size_t numChars = alea.leq(256);
            const size_t charSize = 1+alea.leq(11);
            mgr.prepare(numChars,charSize);
        }
    }
#endif
    
}
Y_UTEST_DONE()

