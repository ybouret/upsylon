#include "y/alchemy/eqs/db.hpp"
#include "y/alchemy/reactor.hpp"

#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Alchemy;

Y_UTEST(eqdb)
{
    EqDB &edb = EqDB::instance();

    std::cerr << "#edb=" << edb->size() << std::endl;

    std::cerr << *edb << std::endl;

    Library    lib;
    Equilibria eqs;
    Lua::VM    vm = new Lua::State();

    Strings keys;
    edb->collect_keys(keys);
    std::cerr << "keys=" << keys << std::endl;
    for(size_t i=1;i<=keys.size();++i)
    {
        eqs.parse(edb[keys[i]],lib,vm);
    }
    std::cerr << lib << std::endl;
    std::cerr << eqs << std::endl;

    Reactor cs(lib,eqs,Equilibrium::Minimal);


}
Y_UTEST_DONE()
