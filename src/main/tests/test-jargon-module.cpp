#include "y/jargon/module.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

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
        Token          content(cache);
        Module::Handle handle =  Module::OpenFile(cache,argv[1]);
        Char          *ch     = 0;
        while( NULL != ( ch=handle->read()) )
        {
            content.push_back( ch );
        }
        std::cerr << "#token=" << content.size << std::endl;
        
    }
    
    std::cerr << "#cache=" << cache->size << std::endl;

    
}
Y_UTEST_DONE()

