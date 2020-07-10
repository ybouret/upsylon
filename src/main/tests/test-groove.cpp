#include "y/memory/groove.hpp"
#include "y/utest/run.hpp"
#include "y/type/spec.hpp"

using namespace upsylon;

namespace
{
    static inline
    void display_groove(   memory::groove &g )
    {
        std::cerr << "g.bytes=" << g.bytes << " @" << type_name_for( g.tid() ) << std::endl;
    }
}

Y_UTEST(groove)
{

    {
        memory::groove g;
        display_groove(g);
        for(size_t iter=0;iter<1000;++iter)
        {
            g.acquire( alea.leq(32) );
        }
        display_groove(g);
    }

    {
        memory::groove g;
        g.make<string>();   display_groove(g);
        g.make<float>();    display_groove(g);
        g.release();        display_groove(g);
        g.make<double>();   display_groove(g);

    }
    std::cerr << "sizeof(groove)=" << sizeof(memory::groove) << std::endl;
}
Y_UTEST_DONE()

