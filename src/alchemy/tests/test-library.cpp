
#include "y/alchemy/library.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;
using namespace Alchemy;


Y_UTEST(library)
{

    Library       lib;
    const Species &h = lib("H+",1);  std::cerr << "add " << h << std::endl;
    const Species &w = lib("HO-",-1); std::cerr << "add " << w << std::endl;

    std::cerr << "#lib=" << lib->size() << std::endl;
    Y_CHECK(lib.owns(h));
    Y_CHECK(lib.owns(w));
    std::cerr << lib << std::endl;

    vector<double> C(lib->size(),1.1);
    lib.display(std::cerr,C) << std::endl;

    for(const Species::Node *node = lib->head();node;node=node->next)
    {
        std::cerr << **node << std::endl;
    }
    
    
}
Y_UTEST_DONE()

