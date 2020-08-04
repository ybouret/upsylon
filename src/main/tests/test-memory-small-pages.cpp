
#include "y/memory/small/pages.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/string/convert.hpp"

#include <iomanip>

using namespace upsylon;
using namespace memory;

#define SHOW_SMALL_PAGE(FIELD) std::cerr <<   #FIELD " = " << std::setw(6) << p.FIELD << std::endl

Y_UTEST(small_pages)
{
    Y_UTEST_SIZEOF(small::page);
    Y_CHECK(sizeof(small::page) == small::pages::min_page_size );


    size_t     large_size = 10000;
    if(argc>1) large_size = string_convert::to<size_t>(argv[1],"large_size");

    small::pages p(large_size);

    SHOW_SMALL_PAGE(min_page_size);
    SHOW_SMALL_PAGE(min_page_iln2);
    SHOW_SMALL_PAGE(max_page_size);
    SHOW_SMALL_PAGE(max_page_iln2);
    SHOW_SMALL_PAGE(deposit_count);
    SHOW_SMALL_PAGE(full_deposits);
    

}
Y_UTEST_DONE()

