#include "y/type/rtti.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;

Y_UTEST(rtti)
{
    

    rtti::repo &types = rtti::repo::instance();

    types.use<int>();
    types.use<int32_t>();
    types.use<int32_t>("int32_t");
    types.use<int>("int");
    types.use<int32_t>("dword");

    types.use<short>();
    types.use<int16_t>();
    types.use<int16_t>("int16_t");
    types.use<short>("short");
    types.use<short>("word");

    
    std::cerr << "db.size=" << types.db.size() << std::endl;
    std::cerr << "id.size=" << types.id.size() << std::endl;

    std::cerr << "by uuid" << std::endl;
    for(rtti::db_iter it=types.db.begin();it!=types.db.end();++it)
    {
        const rtti &tid = **it;
        std::cerr << tid << std::endl;
    }
    
    std::cerr << "by alias" << std::endl;
    for(rtti::id_iter it=types.id.begin();it!=types.id.end();++it)
    {
        const rtti &tid = **it;
        std::cerr << tid << " <=" << it.get().data.key() << std::endl;
    }

    
    Y_UTEST_SIZEOF(rtti);
    Y_UTEST_SIZEOF(rtti::alias);
    Y_UTEST_SIZEOF(string);

}
Y_UTEST_DONE()



