#include "y/jargon/module.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/ios/icstream.hpp"

using namespace upsylon;
using namespace Jargon;

Y_UTEST(module)
{
    
    Y_UTEST_SIZEOF(Jargon::Context);
    Y_UTEST_SIZEOF(Jargon::Char);
    Y_UTEST_SIZEOF(Jargon::Char::Pool);
    Y_UTEST_SIZEOF(Jargon::Token);
    std::cerr << std::endl;
    
    Cache cache = new Char::Pool();
    
    if( argc > 1)
    {
        {
            Token          content(cache);
            Module::Handle handle =  Module::OpenFile(cache,argv[1]);
            Char          *ch     = 0;
            while( NULL != ( ch=handle->getChar()) )
            {
                content.push_back( ch );
            }
            std::cerr << "#token=" << content.size << std::endl;
            Y_ASSERT(NULL==handle->getChar());
            
        }
        std::cerr << "#cache=" << cache->size << std::endl;

        cache->release();
        {
            const string fileName = argv[1];
            if( Y_STDIN != fileName )
            {
                ios::icstream fp( fileName );
                string        line;
                while(fp.gets(line))
                {
                    Module::Handle handle = Module::OpenData(cache,line);
                    Token          content(cache);
                    Char          *ch     = 0;
                    while( NULL != (ch=handle->getChar()) )
                    {
                        content.push_back(ch);
                    }
                    Y_ASSERT(content.size==line.size());
                }
            }
        }
        
    }
    std::cerr << "#cache=" << cache->size << std::endl;


    
}
Y_UTEST_DONE()

