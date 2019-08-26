#include "y/ios/osstream.hpp"
#include "y/utest/run.hpp"
#include "y/type/ints.hpp"
#include "y/string.hpp"
#include "y/ios/imstream.hpp"

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
            std::cerr << "output.size=" << output.size() << "/" << count << std::endl;
            ios::imstream inp(output);
            Y_CHECK( 0 == inp.read_upack<T>() );
        }

        {
            count = 0;
            output.clear();
            ios::osstream fp(output);
            fp.emit_upack<T>( limit_of<T>::maximum, &count );
            std::cerr << "output.size=" << output.size() << "/" << count << std::endl;
            ios::imstream inp(output);
            Y_CHECK( limit_of<T>::maximum == inp.read_upack<T>() );
        }






    }
}

Y_UTEST(upack)
{
    do_upack<uint8_t>();
    do_upack<uint16_t>();
    do_upack<uint32_t>();
    do_upack<uint64_t>();

}
Y_UTEST_DONE()

