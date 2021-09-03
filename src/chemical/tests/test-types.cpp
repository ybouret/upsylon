
#include "y/chemical/reactor.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using  namespace upsylon;

Y_UTEST(types)
{
    Y_UTEST_SIZEOF(Chemical::Object);
    Y_UTEST_SIZEOF(Chemical::Indexed);
    Y_UTEST_SIZEOF(Chemical::Labeled);
    Y_UTEST_SIZEOF(Chemical::Species);
    Y_UTEST_SIZEOF(Chemical::Freezable);
    Y_UTEST_SIZEOF(Chemical::Freezable::Latch);

    Y_UTEST_SIZEOF(Chemical::Library);
    Y_UTEST_SIZEOF(Chemical::Actor);
    Y_UTEST_SIZEOF(Chemical::Actor::Map);
    Y_UTEST_SIZEOF(Chemical::Actors);

    Y_UTEST_SIZEOF(Chemical::Equilibrium);
    Y_UTEST_SIZEOF(Chemical::Equilibria);

    Y_UTEST_SIZEOF(Chemical::Leading);
    Y_UTEST_SIZEOF(Chemical::Seeking);

    Y_UTEST_SIZEOF(Chemical::Reactor);

}
Y_UTEST_DONE()
