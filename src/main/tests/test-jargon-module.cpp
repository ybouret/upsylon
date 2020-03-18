#include "y/jargon/module.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/ios/icstream.hpp"
#include "y/hashing/sha1.hpp"

using namespace upsylon;
using namespace Jargon;

Y_UTEST(jargon_module)
{
    
    Y_UTEST_SIZEOF(Jargon::Context);
    Y_UTEST_SIZEOF(Jargon::Char);
    Y_UTEST_SIZEOF(Jargon::Char::Pool);
    Y_UTEST_SIZEOF(Jargon::Token);
    std::cerr << std::endl;
    
    Char::Cache cache = new Char::Pool();
    
    if( argc > 1)
    {
        {
            hashing::sha1  H;
            Token          content(cache);
            Module::Handle handle =  Module::OpenFile(cache,argv[1]);
            Char          *ch     = 0;
            H.set();
            while( NULL != ( ch=handle->read()) )
            {
                content.push_back( ch );
                H.run_type(content.tail->code);
            }
            std::cerr << "#token=" << content.size << std::endl;
            const digest md0 = H.md();
            Y_ASSERT(NULL==handle->read());
            
            handle->unread(content);
            H.set();
            while( NULL != ( ch=handle->read()) )
            {
                content.push_back( ch );
                H.run_type(content.tail->code);
            }
            std::cerr << "#token=" << content.size << std::endl;
            const digest md1 = H.md();
            Y_CHECK(md0==md1);
            
            handle->uncopy(content);
            content.erase();
            H.set();
            while( NULL != ( ch=handle->read()) )
            {
                content.push_back( ch );
                H.run_type(content.tail->code);
            }
            std::cerr << "#token=" << content.size << std::endl;
            const digest md2 = H.md();
            Y_CHECK(md0==md2);
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
                    while( NULL != (ch=handle->read()) )
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

