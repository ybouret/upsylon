


#include "y/jive/source.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;
using namespace Jive;

Y_UTEST(jive_source)
{
    Char::Reserve(128*1024);
    if(argc>1)
    {
        Source source( Module::OpenFile(argv[argc-1]) );
        for(int i=argc-2;i>0;--i)
        {
            source.push( Module::OpenFile(argv[i]) );
        }

        Char::List content;
        {
            Char *ch = 0;
            while( (ch=source.get()) )
            {
                content.push_back(ch);
            }
        }
        std::cerr << content << std::endl;
        std::cerr << "#content=" << content.size << std::endl;
        const size_t nr = content.size;
        source.unget(content);
        Y_CHECK(nr==source.in_cache());
    }



}
Y_UTEST_DONE()
