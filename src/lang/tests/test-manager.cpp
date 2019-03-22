#include "y/lang/dynamo/manager.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Lang;

Y_UTEST(manager)
{
    DynamoManager   &dynamo = DynamoManager::instance();
    DynamoGenerator &g      = dynamo.generator();
    g.clear();

    DynamoLoader    &l      = dynamo.loader();
    std::cerr << l.name << std::endl;

}
Y_UTEST_DONE()

