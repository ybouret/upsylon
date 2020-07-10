#include "y/memory/grooves.hpp"
#include "y/utest/run.hpp"
#include "y/type/spec.hpp"

using namespace upsylon;

namespace {

    template <typename U,typename V>
    class Zap1
    {
    public:
        const U u;
        const V v;
        Zap1(const U &argU, const V &argV) : u(argU), v(argV) {}
        ~Zap1() throw() {}


    private:
        Y_DISABLE_COPY_AND_ASSIGN(Zap1);
    };

}

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
        g.make<string>(hello);      std::cerr << g << std::endl;
        std::cerr << g.as<string>() << std::endl;
        g.make<const string>(hello); std::cerr << g << std::endl;
        std::cerr << g.as<string>() << std::endl;

        g.build<string,const char *>( "toto" );
        std::cerr << g << std::endl;
        g.build< Zap1<int,short>, int, short >(3,4);
        std::cerr << g << std::endl;

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
    std::cerr << G[1].as<int>() << std::endl;
    std::cerr << G[2].as<float>() << std::endl;
    std::cerr << G[3].as<double>() << std::endl;



}
Y_UTEST_DONE()

