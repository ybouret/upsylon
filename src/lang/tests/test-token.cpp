#include "y/lang/token.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Lang;


#define SHOW(CLASS) std::cerr << "sizeof(" #CLASS << ")=" << sizeof(CLASS) << std::endl

Y_UTEST(token)
{

    SHOW(Lang::Char);
    SHOW(Lang::Token);
    SHOW(Lang::Char::Pool);
    SHOW(Lang::Char::List);
    SHOW(Lang::Manager);


}
Y_UTEST_DONE()


