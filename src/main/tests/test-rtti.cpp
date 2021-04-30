#include "y/type/rtti.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(rtti)
{

    rtti::repo &types = rtti::repo::instance();

    types.use<int>();
    types.use<int32_t>();
    types.use<int32_t>("dword");

    std::cerr << "db.size=" << types.db.size() << std::endl;
    std::cerr << "id.size=" << types.id.size() << std::endl;

    for(rtti::db_iter it=types.db.begin();it!=types.db.end();++it)
    {
        const rtti &tid = **it;
        std::cerr << tid.uuid << std::endl;
    }


}
Y_UTEST_DONE()



