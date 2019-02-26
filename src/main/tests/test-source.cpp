
#include "y/lang/source.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"

using namespace upsylon;
using namespace Lang;

Y_UTEST(source)
{

    Module *module = NULL;
    if(argc>1)
    {
        const string filename = argv[1];
        if( "NULL" == filename )
        {
            return 0;
        }
        module = Module::OpenFile(filename);
    }
    else
    {
        module = Module::OpenSTDIN();
    }

    Source source(module);
    {
        {
            Token  content;
            source.prefetch(4);

            Char *ch = NULL;
            while( NULL != (ch=source.get()) )
            {
                content.push_back(ch);
            }
            std::cerr << "#chars_level1=" << content.size << std::endl;
            source.ungetCopy(content);
        }
        Token content_copy;
        {
            Char *ch = NULL;
            while( NULL != (ch=source.get()) )
            {
                content_copy.push_back(ch);
            }
        }
        std::cerr << "#chars_level2=" << content_copy.size << std::endl;
        std::cerr << "content=" << content_copy << std::endl;
    }
    

}
Y_UTEST_DONE()
