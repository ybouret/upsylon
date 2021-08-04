#include "y/alchemy/reactor.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"


using namespace upsylon;
using namespace Alchemy;


Y_UTEST(types)
{
    std::cerr << "Species:" << std::endl;
    Y_UTEST_SIZEOF(Species);
    Y_UTEST_SIZEOF(Species::Pointer);
    Y_UTEST_SIZEOF(Species::Set);
    std::cerr << std::endl;

    std::cerr << "Library:" << std::endl;
    Y_UTEST_SIZEOF(Library);
    std::cerr << std::endl;

    std::cerr << "Actor:" << std::endl;
    Y_UTEST_SIZEOF(Actor);
    Y_UTEST_SIZEOF(Actor::Set);
    std::cerr << std::endl;

    std::cerr << "Actors:" << std::endl;
    Y_UTEST_SIZEOF(Actors);
    std::cerr << std::endl;

    std::cerr << "Equilibrium:" << std::endl;
    Y_UTEST_SIZEOF(Equilibrium);
    Y_UTEST_SIZEOF(Equilibrium::Pointer);
    Y_UTEST_SIZEOF(Equilibrium::Set);
    std::cerr << std::endl;

    std::cerr << "Equilibria:" << std::endl;
    Y_UTEST_SIZEOF(Equilibria);
    std::cerr << std::endl;

    std::cerr << "Reactor:" << std::endl;
    Y_UTEST_SIZEOF(Reactor);
    std::cerr << std::endl;


}
Y_UTEST_DONE()

