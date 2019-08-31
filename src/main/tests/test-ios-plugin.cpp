#include "y/ios/plugin.hpp"
#include "y/ios/ovstream.hpp"
#include "y/ios/imstream.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"

using namespace upsylon;


namespace
{
    template <typename T>
    static inline void do_plugin( const ios::plugin &plg )
    {
        std::cerr << "I/O for <" << typeid(T).name() << ">" << std::endl;
        ios::ovstream data;
        for(size_t cycle=0;cycle<8;++cycle)
        {
            data.free();
            size_t        ns   = 0;
            size_t        no   = 0;
            for(size_t i=10+alea.leq(100);i>0;--i)
            {
                const T tmp = support::get<T>();
                plg.save(data, &tmp, &ns );
                ++no;
            }
            std::cerr << "\tWritten #objs=" << no << " with #bytes=" << ns << std::endl;

            ios::imstream fp(data);
            size_t ni = 0;
            size_t nl = 0;
            while(fp.is_active())
            {
                T tmp;
                plg.load(fp,&tmp,&nl);
                ++ni;
            }
            std::cerr << "\tLoaded  #objs=" << ni << " with #bytes=" << nl << std::endl;
            Y_ASSERT(no==ni);
            Y_ASSERT(nl==ns);
        }

    }
}

Y_UTEST(ios_plugin)
{
    {
        ios::plugin_raw<double> plg;
        do_plugin<double>( plg );
    }

    {
        ios::plugin_net<uint32_t> plg;
        do_plugin<uint32_t>( plg );
    }

    {
        ios::plugin_srz<string> plg;
        do_plugin<string>( plg );
    }
}
Y_UTEST_DONE()



