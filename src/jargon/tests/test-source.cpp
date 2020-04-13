#include "y/jargon/source.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Jargon;

Y_UTEST(source)
{
    
    
    if(argc>1)
    {
        int j=argc-1;
        Source source(Module::OpenFile(argv[j]));
        while(j>1) source.load( Module::OpenFile(argv[--j]));
        
        Token content;
        Char *ch = 0;
        while( NULL != (ch=source.get() ))
        {
            content.push_back(ch);
        }
        Y_CHECK(NULL==source.get());
        std::cerr << "#content=" << content.size << std::endl;
        
        source.unget(content);
        {
            string line;
            while( source.gets(line) )
            {
                std::cerr << line << std::endl;
            }
        }
    }
    
    
    
}
Y_UTEST_DONE()
