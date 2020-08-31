#include "y/jive/pattern/leading.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;
using namespace Jive;

Y_UTEST(jive_leading)
{
    Y_UTEST_SIZEOF(Leading);
    
    LeadingChars L;
    std::cerr << "L=" << L << std::endl;
    L.insert('c');
    std::cerr << "L=" << L << std::endl;
    L.insert('b');
    std::cerr << "L=" << L << std::endl;
    L.insert('a');
    std::cerr << "L=" << L << std::endl;

}
Y_UTEST_DONE()

