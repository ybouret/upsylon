#include "y/concurrent/threads.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace
{
    class protocol
    {
    public:

        explicit protocol()
        {
            
        }

        virtual ~protocol() throw()
        {
        }

        static void call( void *data, parallel &context, lockable &access )
        {
            assert(data);
            static_cast<protocol *>(data)->run(context,access);
        }

        void run(parallel &context, lockable &access)
        {
            {
                Y_LOCK(access);
                std::cerr << "protocol in " << context.label << std::endl;
            }
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(protocol);
    };
}
            
#include <cstring>
#include "y/string/env.hpp"
#include "y/sequence/vector.hpp"

Y_UTEST(threads)
{
    std::cerr << "sizeof(thread)=" << sizeof(concurrent::thread) << std::endl;

    bool run = true;
    if(argc>1 && !strcmp(argv[1],"false"))
    {
        run = false;
    }

    environment::set(Y_THREADS_VERBOSITY, "1");
    concurrent::threads engine;

    if(run)
    {
        protocol proto;
        engine.run( protocol::call, &proto );
    }

    engine.make<int>();
    const size_t n = engine.size();
    for(size_t i=n;i>0;--i)
    {
        engine[i]._<int>() = int(i);
    }

    vector<double> res(n,0);
    concurrent::executor::dowload<int>::to(res,engine);
    std::cerr << "res=" << res << std::endl;

    vector<int16_t> dat(n,1);
    concurrent::executor::upload<int>::to(engine,dat);



}
Y_UTEST_DONE()
