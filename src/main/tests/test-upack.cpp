#include "y/ios/osstream.hpp"
#include "y/utest/run.hpp"
#include "y/type/ints.hpp"
#include "y/string.hpp"
#include "y/ios/imstream.hpp"

#include "y/ios/upack.hpp"

using namespace upsylon;

namespace
{
    template <typename T>
    static inline
    void do_upack()
    {

        string output;
        size_t count = 0;

        {
            count = 0;
            output.clear();
            ios::osstream fp(output);
            fp.emit_upack<T>( 0, &count );
            std::cerr << "output.size=" << output.size() << "/" << count << " ";
            ios::imstream inp(output);
            Y_CHECK( 0 == inp.read_upack<T>() );
        }

        {
            count = 0;
            output.clear();
            ios::osstream fp(output);
            fp.emit_upack<T>( limit_of<T>::maximum, &count );
            std::cerr << "output.size=" << output.size() << "/" << count << " ";
            ios::imstream inp(output);
            Y_CHECK( limit_of<T>::maximum == inp.read_upack<T>() );
        }

        std::cerr << "sizeof(upack)  =" << sizeof(ios::upack<T>)    << std::endl;
        std::cerr << "upack.type_size=" << ios::upack<T>::word_size << std::endl;
        std::cerr << "upack.requested=" << ios::upack<T>::requested << std::endl;
        std::cerr << "upack.workspace=" << ios::upack<T>::workspace << std::endl;

        ios::upack<T> pak;

        std::cerr << "..." << std::endl;
        for(size_t iter=0;iter<1024*128;++iter)
        {
            count = 0;
            output.clear();
            ios::osstream fp(output);
            const T tmp = alea.full<T>();
            fp.emit_upack<T>( tmp, &count );
            ios::imstream inp(output);
            Y_ASSERT( tmp == inp.read_upack<T>() );
            pak.encode(tmp);
            Y_ASSERT(pak.size==count);
            Y_ASSERT( 0 == memcmp( *output, pak.ro(), pak.size ) );
            assert( tmp == pak.template decode<T>() );
        }



    }
}

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

}
Y_UTEST_DONE()

