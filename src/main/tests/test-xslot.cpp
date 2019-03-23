#include "y/memory/xslot.hpp"
#include "y/utest/run.hpp"

#include "y/memory/pooled.hpp"

using namespace upsylon;
using namespace memory;

namespace  {
    static inline void display_xs( const xslot_type &xs )
    {
        std::cerr << "size=" << xs.size << std::endl;
    }
}

Y_UTEST(xslot)
{
    std::cerr << "sizeof(xslot<global>)=" << sizeof(xslot<global::allocator>) << std::endl;
    std::cerr << "sizeof(xslot<pooled>)=" << sizeof(xslot<pooled::allocator>) << std::endl;

    xslot<>       xdefault0;     display_xs( xdefault0 );
    xslot<>       xdefault1(10); display_xs( xdefault1 );
    xslot<pooled> xpooled0;      display_xs(xpooled0);
    xslot<pooled> xpooled1(10);  display_xs(xpooled1);

}
Y_UTEST_DONE()

