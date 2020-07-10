#include "y/memory/xslot.hpp"
#include "y/utest/run.hpp"

#include "y/memory/pooled.hpp"
#include "y/string.hpp"

using namespace upsylon;
using namespace memory;

namespace  {
    static inline void display_xs( const xslot_type &xs )
    {
        std::cerr << "size=" << xs.size << std::endl;
        Y_ASSERT(xs.is_zeroed());
        Y_ASSERT(!xs.is_cplusplus());
    }

    class dummy
    {
    public:
        const int data;
        dummy() : data(0)
        {
            std::cerr << "New Dummy0@" << data << std::endl;
        }

        dummy(const int z) : data(z)
        {
            std::cerr << "New Dummy1@" << data << std::endl;
        }

        ~dummy() throw()
        {
            std::cerr << "Zap Dummy@" << data << std::endl;
        }



    private:
        Y_DISABLE_COPY_AND_ASSIGN(dummy);
    };
}

Y_UTEST(xslot)
{
    std::cerr << "sizeof(xslot<global>)=" << sizeof(xslot<global>) << std::endl;
    std::cerr << "sizeof(xslot<pooled>)=" << sizeof(xslot<pooled>) << std::endl;

    std::cerr << "init global:" << std::endl;
    xslot<>       xdefault0;     display_xs( xdefault0 );
    xslot<>       xdefault1(10); display_xs( xdefault1 );

    std::cerr << "init pooled:" << std::endl;
    xslot<pooled> xpooled0;      display_xs(xpooled0);
    xslot<pooled> xpooled1(10);  display_xs(xpooled1); Y_ASSERT(xpooled1.size>=10);

    std::cerr << "acquire global:" << std::endl;
    xdefault0.acquire(21); display_xs( xdefault0 ); Y_ASSERT(xdefault0.size>=21);
    xdefault1.acquire(6);  display_xs( xdefault1 ); Y_ASSERT(xdefault1.size>=6);

    std::cerr << "acquire pooled: " << std::endl;
    xpooled0.acquire(21);  display_xs( xpooled0  ); Y_ASSERT( xpooled0.size >= 21);
    xpooled1.acquire(43);  display_xs( xpooled1  ); Y_ASSERT( xpooled1.size >= 43);

    


    std::cerr << "build string..." << std::endl;
    xdefault0.build<string>(); Y_ASSERT(xdefault0.has_bytes_for<string>());
    xdefault1.build<string>(); Y_ASSERT(xdefault1.has_bytes_for<string>());
    xpooled0.build<string>();  Y_ASSERT(xpooled0.has_bytes_for<string>());
    xpooled1.build<string>();  Y_ASSERT(xpooled1.has_bytes_for<string>());


    std::cerr << "build dummy..." << std::endl;
    xdefault0.build<dummy>();
    xdefault1.build<dummy>();
    xpooled0.build<dummy>();
    xpooled1.build<dummy>();

    std::cerr << "build strings..." << std::endl;
    {
        const string arg = "hello";
        xdefault0.build_from<string>(arg);
        xdefault1.build_from<string>(arg);
        xpooled0.build_from<string>(arg);
        xpooled1.build_from<string>(arg);
    }

    std::cerr << "build dummys..." << std::endl;

    xdefault0.build<dummy,int>(1);
    xdefault1.build<dummy,char>(2);
    xpooled0.build<dummy,short>(3);
    xpooled1.build<dummy,long>(4);

}
Y_UTEST_DONE()

