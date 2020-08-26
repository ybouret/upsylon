
#include "y/jive/char.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;
using namespace Jive;

Y_UTEST(jive_char)
{
    //concurrent::singleton::verbose = true;
    Y_UTEST_SIZEOF(Context);
    Y_UTEST_SIZEOF(Char);


    Context ctx1( "hello" );
    Context ctx2( program );

    Tags::Display();

    {
    Char::List l1;
    for(size_t i=alea.leq(32)+1;i>0;--i)
    {
        l1.add(ctx1,alea.range<char>('a','z'));
    }
    std::cerr << "l1='" << l1 << "'" << std::endl;
    {
        const Char::List l2(l1);
        std::cerr << "l2='" << l2 << "'" << std::endl;
    }
    Char::List l3;
    std::cerr << "l3='" << l3 << "'" << std::endl;
    l3 = l1;
    std::cerr << "l3='" << l3 << "'" << std::endl;
    }

    Char::Supply &cc = Char::Supply::instance();
    std::cerr << "#zchars: " << cc.zCache().size << std::endl;

}
Y_UTEST_DONE()

