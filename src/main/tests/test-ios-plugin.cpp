#include "y/ios/plugins.hpp"
#include "y/ios/ovstream.hpp"
#include "y/ios/imstream.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include <typeinfo>

using namespace upsylon;


namespace
{
    template <typename T>
    static inline void do_plugin( ios::plugin &plg )
    {
        std::cerr << "I/O for <" << typeid(T).name() << ">" << std::endl;
        ios::ovstream data;
        for(size_t cycle=0;cycle<16;++cycle)
        {
            data.free();
            size_t        ns   = 0;
            size_t        no   = 0;
            for(size_t i=10+alea.leq(200);i>0;--i)
            {
                const T tmp = support::get<T>();
                ns += plg.save(data, &tmp);
                ++no;
            }
            std::cerr << "\tWritten #objs=" << no << " with #bytes=" << ns << std::endl;

            ios::imstream fp(data);
            size_t ni = 0;
            size_t nl = 0;
            while(fp.is_active())
            {
                T tmp;
                nl += plg.load(fp,&tmp);
                ++ni;
            }
            std::cerr << "\tLoaded  #objs=" << ni << " with #bytes=" << nl << std::endl;
            Y_ASSERT(no==ni);
            Y_ASSERT(nl==ns);
        }
    }

    template <typename T>
    static inline void do_with( const ios::plugins &pdb )
    {
        ios::plugin::pointer plg = pdb.query<T>();
        do_plugin<T>(*plg);
    }

}

Y_UTEST(ios_plugin)
{
    std::cerr << "-- testing direct plugins" << std::endl;

    {
        ios::raw_plugin<double> plg;
        do_plugin<double>( plg );
    }

    {
        ios::nbo_plugin<uint32_t> plg;
        do_plugin<uint32_t>( plg );
    }

    {
        ios::srz_plugin<string> plg;
        do_plugin<string>( plg );
    }

    {
        ios::srz_plugin<mpn> plg;
        do_plugin<mpn>( plg );
    }

    {
        ios::srz_plugin<mpz> plg;
        do_plugin<mpz>( plg );
    }
    
    {
        ios::srz_plugin<mpq> plg;
        do_plugin<mpq>( plg );
    }

    std::cerr << "-- testing database" << std::endl;

    ios::plugins pdb;
    pdb.declare_raw<char>();
    pdb.declare_raw<unsigned char>();
    pdb.declare_raw<int8_t>();
    pdb.declare_raw<uint8_t>();

    pdb.declare_nbo<short>();
    pdb.declare_nbo<unsigned short>();
    pdb.declare_nbo<int16_t>();
    pdb.declare_nbo<uint16_t>();

    pdb.declare_nbo<int>();
    pdb.declare_nbo<unsigned int>();
    pdb.declare_nbo<long>();
    pdb.declare_nbo<unsigned long>();

    pdb.declare_nbo<uint32_t>();
    pdb.declare_nbo<int32_t>();

    pdb.declare_nbo<int64_t>();
    pdb.declare_nbo<uint64_t>();


    pdb.declare_srz<string>();
    pdb.declare_srz<mpn>();
    pdb.declare_srz<mpz>();
    pdb.declare_srz<mpq>();

    pdb.declare_raw<double>();
    pdb.declare_raw<float>();
    pdb.declare_raw< complex<float>  >();
    pdb.declare_raw< complex<double> >();


    for( ios::plugins::iterator i=pdb.begin(); i!=pdb.end(); ++i)
    {
        const uint32_t uuid = (*i).plg->uuid;
        const string   sid  = vformat("0x%08x",uuid);
        std::cerr << "<" << i->key().name() << "> : " << sid << std::endl;
    }

    do_with<uint32_t>( pdb );
    do_with< complex<float> >( pdb );
    do_with< string >( pdb );
    do_with< mpq >( pdb );

}
Y_UTEST_DONE()



