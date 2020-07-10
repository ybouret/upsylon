#include "y/memory/grooves.hpp"
#include "y/utest/run.hpp"
#include "y/type/spec.hpp"

using namespace upsylon;


Y_UTEST(groove)
{

    {
        memory::groove g;
        std::cerr << g << std::endl;
        for(size_t iter=0;iter<1000;++iter)
        {
            g.prepare( alea.leq(32) );
        }
        std::cerr << g << std::endl;
    }

    {
        memory::groove g;
        g.make<string>();   std::cerr << g << std::endl;
        g.make<float>();    std::cerr << g << std::endl;
        g.release();        std::cerr << g << std::endl;
        g.make<double>();   std::cerr << g << std::endl;
        const string hello = "world";
        g.make(hello);      std::cerr << g << std::endl;
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

    G.as<int>(1)    = -1;
    G.as<float>(2)  = 1.0f;
    G.as<double>(3) = 3.14;
    
    std::cerr << G << std::endl;



}
Y_UTEST_DONE()

