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
    Y_UTEST_SIZEOF(Jargon::Token);
    std::cerr << std::endl;
    
    
    if( argc > 1)
    {
        {
            Token          content;
            Module::Handle handle =  Module::OpenFile(argv[1]);
            Char          *ch     = 0;
            while( NULL != ( ch=handle->getChar()) )
            {
                content.push_back( ch );
            }
            std::cerr << "#token=" << content.size << std::endl;
            Y_ASSERT(NULL==handle->getChar());
            
        }

        {
            const string fileName = argv[1];
            if( Y_STDIN != fileName )
            {
                ios::icstream fp( fileName );
                string        line;
                while(fp.gets(line))
                {
                    Module::Handle handle = Module::OpenData(line);
                    Token          content;
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


    
}
Y_UTEST_DONE()

