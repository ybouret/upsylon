#include "y/ios/tools/serializable.hpp"
#include "y/hashing/sha1.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"

using namespace upsylon;

namespace {

    template <typename T>
    static inline void test_srz(  )
    {

        const T                  t = support::get<T>();
        const ios::serializable &s = t;
        hashing::sha1            H;
        std::cerr << "org=[" << t << "]" << std::endl;

        const string b64 = s.to_base64();
        std::cerr << "b64=[" << b64 << "]" << std::endl;

        const size_t hkey = ios::serialized::key<size_t>(s,H);
        std::cerr << "hkey=" << hkey << std::endl;
    }

}

Y_UTEST(serialize_tools)
{
    test_srz<string>();
}
Y_UTEST_DONE()


