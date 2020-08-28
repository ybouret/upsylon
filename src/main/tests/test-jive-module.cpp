#include "y/jive/module.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;
using namespace Jive;

Y_UTEST(jive_module)
{
    Char::Reserve(128*1024);
    
    try
    {
        Module::Handle module = Module::OpenFile("toto");
    }
    catch(const exception &e)
    {
        std::cerr << e.what() << " : " << e.when() << std::endl;
    }
    catch(...)
    {
        throw;
    }

    if(argc>1)
    {
        Module::Handle module = Module::OpenFile(argv[1]);
        std::cerr << "open " << module->tag << std::endl;

        Char::List chars;
        Char      *ch = 0;
        while( (ch=module->getChar()) )
        {
            chars << ch;
        }
        std::cerr << "#chars=" << chars.size << std::endl;
        std::cerr << chars << std::endl;
    }

}
Y_UTEST_DONE()

