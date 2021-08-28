
#include "y/chemical/library.hpp"
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


}
Y_UTEST_DONE()

