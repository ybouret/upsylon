#include "y/memory/grooves.hpp"
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
            g.prepare( alea.leq(32) );
        }
        display_groove(g);
    }

    {
        memory::groove g;
        g.make<string>();   display_groove(g);
        g.make<float>();    display_groove(g);
        g.release();        display_groove(g);
        g.make<double>();   display_groove(g);
        const string hello = "world";
        g.make(hello); display_groove(g);
        std::cerr << g.as<string>() << std::endl;
    }
    std::cerr << "sizeof(groove)=" << sizeof(memory::groove) << std::endl;

    memory::grooves G;
    G.acquire(4);
    G.make<float>(2);
    std::cerr << "#grooves=" << G.size() << std::endl;
    std::cerr << G << std::endl;
    G.dismiss();
    std::cerr << G << std::endl;
    G.make<double,string>();
    std::cerr << G << std::endl;

    G.make<int,float,double>();
    std::cerr << G << std::endl;



}
Y_UTEST_DONE()

