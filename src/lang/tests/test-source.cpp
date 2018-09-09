
#include "y/lang/source.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"

using namespace upsylon;
using namespace Lang;

Y_UTEST(source)
{
    if(argc>1)
    {
        const string filename = argv[1];
        if(filename=="NULL") return 0;
        
        Source source( Module::OpenFile(argv[1]) );
        {
            Token  content;
            source.prefetch(4);

            Char *ch = NULL;
            while( NULL != (ch=source.get()) )
            {
                content.push_back(ch);
            }
            std::cerr << "#chars=" << content.size << std::endl;
            source.unget_copy(content);
        }
        Token content_copy;
        {
            Char *ch = NULL;
            while( NULL != (ch=source.get()) )
            {
                content_copy.push_back(ch);
            }
        }
    }
}
Y_UTEST_DONE()
