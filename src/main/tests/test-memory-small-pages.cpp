
#include "y/memory/small/pages.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/string/convert.hpp"

#include <iomanip>

using namespace upsylon;
using namespace memory;

 
#define SHOW(FIELD) std::cerr << #FIELD " = " << small::pages::FIELD << std::endl

Y_UTEST(small_pages)
{
    
    size_t     large_size = 10000;
    if(argc>1) large_size = string_convert::to<size_t>(argv[1],"large_size");

    //small::pages p(large_size);
    
    SHOW(min_iln2);
    SHOW(min_size);
    SHOW(max_iln2);
    SHOW(max_size);
    SHOW(required);
    SHOW(_aligned);
    SHOW(in_words);

    

}
Y_UTEST_DONE()

