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
#include "y/concurrent/exec-io.hpp"

Y_UTEST(threads)
{
    std::cerr << "sizeof(thread)=" << sizeof(concurrent::thread) << std::endl;

    bool run = true;
    if(argc>1 && !strcmp(argv[1],"false"))
    {
        run = false;
    }

    environment::set(Y_VERBOSE_THREADS, "1");
    concurrent::threads engine;

    if(run)
    {
        protocol proto;
        engine.run( protocol::call, &proto );
    }

    engine.make<int>(2);
    const size_t n = engine.size();
    for(size_t i=n;i>0;--i)
    {
        engine[i]._<int>() = int(i);
    }

    concurrent::array_of<int>  arr (engine);
    concurrent::xarray_of<int> arr1(engine,1);
    concurrent::xarray_of<int> arr2(engine,2);

    std::cerr << "arr   = " << arr  << std::endl;
    std::cerr << "arr1  = " << arr1 << std::endl;
    std::cerr << "arr2  = " << arr2 << std::endl;


}
Y_UTEST_DONE()
