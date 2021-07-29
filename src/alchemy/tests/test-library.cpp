
#include "y/alchemy/library.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace alchemy;


Y_UTEST(library)
{

    library        lib;
    const species &h = lib("H+",1);  std::cerr << "add " << h.name << std::endl;
    const species &w = lib("HO-",1); std::cerr << "add " << w.name << std::endl;


    for( library::const_iterator it= (*lib).begin(); it!=(*lib).end(); ++it)
    {
        const species &sp = **it;
        lib.prefix(std::cerr,sp) << ": z=" << sp.z << std::endl;
    }

}
Y_UTEST_DONE()

