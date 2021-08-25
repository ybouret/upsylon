

#include "y/alchemy/eqs/db.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Alchemy;

Y_UTEST(eqdb)
{
    EqDB &edb = EqDB::instance();

    std::cerr << "#edb=" << edb->size() << std::endl;

}
Y_UTEST_DONE()
