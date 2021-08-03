

#include "y/alchemy/freezable.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Alchemy;

Y_UTEST(freeze)
{

    Freezable entry;
    entry.freeze();
    try
    {
        entry.freeze("entry");
    }
    catch(const exception &e)
    {
        std::cerr << e.what() << ": " << e.when() << std::endl;
    }
    entry.permit();
}
Y_UTEST_DONE()

