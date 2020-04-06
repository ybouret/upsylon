#include "y/jargon/source.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Jargon;

Y_UTEST(source)
{
    
    Cache cache;
    
    if(argc>1)
    {
        int j=argc-1;
        Source source(Module::OpenFile(cache,argv[j]));
        while(j>1) source.load( Module::OpenFile(cache,argv[--j]));
        
        Token content(cache);
        Char *ch = 0;
        while( NULL != (ch=source.get() ))
        {
            content.push_back(ch);
        }
        Y_CHECK(NULL==source.get());
        std::cerr << "#content=" << content.size << std::endl;
        
        source.unget(content);
        cache->optimize();
        {
            string line;
            while( source.gets(line) )
            {
                std::cerr << line << std::endl;
            }
        }
    }
    
    std::cerr << "#cache=" << cache->size <<  "/bytes=" << cache->size * sizeof(Char) << std::endl;
    cache->maxBytes(1024);
    std::cerr << "#cache=" << cache->size <<  "/bytes=" << cache->size * sizeof(Char) << std::endl;

    
}
Y_UTEST_DONE()
