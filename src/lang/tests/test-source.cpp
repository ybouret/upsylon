
#include "y/lang/source.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"

using namespace upsylon;
using namespace Lang;

Y_UTEST(source)
{
    if(argc>1)
    {
        Input  input = new ios::icstream( argv[1] );
#if 0
        Source source(input);
        {
            source.unget_copy("Hello, World!\n");
        }
        Token  content;
        while(true)
        {
            Char *ch = source.get();
            if(ch)
            {
                content.push_back(ch);
            }
            else
                break;
        }
        std::cerr << content << std::endl;
#endif

    }
}
Y_UTEST_DONE()
