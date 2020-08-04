
#include "y/memory/small/pages.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;
using namespace memory;

Y_UTEST(small_pages)
{
    Y_UTEST_SIZEOF(small::page);
    Y_CHECK(sizeof(small::page) == small::pages::min_page_size );

}
Y_UTEST_DONE()

