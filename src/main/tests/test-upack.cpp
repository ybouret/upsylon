#include "y/ios/osstream.hpp"
#include "y/utest/run.hpp"
#include "y/type/ints.hpp"
#include "y/string.hpp"
#include "y/ios/imstream.hpp"
#include "y/ios/osstream.hpp"

#include "y/ios/upack.hpp"

using namespace upsylon;

namespace
{
    template <typename T>
    static inline
    void do_upack()
    {

        string output;
        size_t shift = 0;
        {
            output.clear();
            ios::osstream fp(output);
            const size_t count = fp.write_upack<T>( 0 );
            std::cerr << "output.size=" << output.size() << "/" << count << " ";
            ios::imstream inp(output);
            T ans = 0;
            Y_CHECK( inp.query_upack(ans,shift) );
            Y_CHECK( 0 == ans );
        }

        {
            output.clear();
            ios::osstream fp(output);
            const size_t count = fp.write_upack<T>( limit_of<T>::maximum);
            std::cerr << "output.size=" << output.size() << "/" << count << " ";
            ios::imstream inp(output);
            T ans = 0;
            Y_CHECK( inp.query_upack(ans,shift) );
            Y_CHECK( limit_of<T>::maximum == ans );
        }

        std::cerr << "sizeof(upack)  =" << sizeof(ios::upack<T>)    << std::endl;
        std::cerr << "upack.type_size=" << ios::upack<T>::word_size << std::endl;
        std::cerr << "upack.requested=" << ios::upack<T>::requested << std::endl;
        std::cerr << "upack.workspace=" << ios::upack<T>::workspace << std::endl;

        ios::upack<T> pak;

        std::cerr << "..." << std::endl;
        for(size_t iter=0;iter<1024*128;++iter)
        {
            output.clear();
            ios::osstream fp(output);
            const T      tmp   = alea.full<T>();
            const size_t count = fp.write_upack<T>(tmp);
            ios::imstream inp(output);
            T            ldd   = 0;
            Y_ASSERT( inp.query_upack(ldd,shift) );
            Y_ASSERT( tmp == ldd);
            pak.encode(tmp);
            Y_ASSERT(pak.size==count);
            Y_ASSERT( 0 == memcmp( *output, pak.ro(), pak.size ) );
            assert( tmp == pak.template decode<T>() );
        }



    }
}

#include <cstdio>
#include "y/code/utils.hpp"

Y_UTEST(upack)
{
    do_upack<uint8_t>();
    do_upack<uint16_t>();
    do_upack<uint32_t>();
    do_upack<uint64_t>();

    std::cerr << "encoding/decoding sizes" << std::endl;

    ios::upack_size szpak;

    for(size_t iter=100000;iter>0;--iter)
    {
        const size_t sz = alea.partial<size_t>( alea.leq(8*sizeof(size_t)) );
        szpak.encode(sz);
        Y_ASSERT(szpak()==sz);
    }

    string output(8,as_capacity,false);
    std::cerr.flush();
    for(unsigned i=0;;++i)
    {
        output.clear();
        {
            ios::osstream fp(output);
            if(fp.write_upack(i)>1)
            {
                break;
            }
        }
        Y_ASSERT(1==output.size());
        const unsigned j = uint8_t(output[0]);
        fprintf(stderr,"%02u-> %4s = %02u\n",i,cchars::visible[j],j);
    }

}
Y_UTEST_DONE()

