

#include "y/jive/module.hpp"
#include "y/jive/token.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;
using namespace Jive;

Y_UTEST(jive_token)
{
    Char::Reserve(128*1024);

    const char data[] = "Hello, World!";

    Module::Handle m = Module::OpenData(program,data);
    Token          t;
    {
        Char *ch=0;
        while( (ch=m->getChar()) ) t << ch;
    }
    std::cerr << "Token='" << t << "'" << std::endl;


}
Y_UTEST_DONE()
