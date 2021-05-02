#include "y/type/rtti.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;

Y_UTEST(rtti)
{
    
    
    Y_UTEST_SIZEOF(rtti);
    Y_UTEST_SIZEOF(rtti::alias);
    Y_UTEST_SIZEOF(rtti::aliases);
    Y_UTEST_SIZEOF(string);
    Y_UTEST_SIZEOF(string);
    
    rtti::repo &types = rtti::repo::instance();
    
    
    std::cerr << "db.size=" << types.db.size() << std::endl;
    std::cerr << "id.size=" << types.id.size() << std::endl;

    std::cerr << std::endl;
    std::cerr << "by uuid" << std::endl;
    for(rtti::db_iter it=types.db.begin();it!=types.db.end();++it)
    {
        const rtti &tid = **it;
        std::cerr << tid << " ==> [" << tid.name() << "] : buffer=[";
        tid.print_to(std::cerr);
        std::cerr << "]" << std::endl;
    }
    
    //types.id.get_root().graphViz("types.dot");

}
Y_UTEST_DONE()



