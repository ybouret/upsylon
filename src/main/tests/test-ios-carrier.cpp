#include "y/ios/carrier/primary.hpp"
#include "y/ios/carrier/network.hpp"
#include "y/ios/ovstream.hpp"
#include "y/ios/imstream.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/code/utils.hpp"

using namespace upsylon;

namespace
{
    template <typename T>
    void test_plain()
    {
        ios::primary_carrier<T> loc;
        ios::network_carrier<T> nbo;
        ios::ovstream           fp;
        
        std::cerr << std::hex;
        const T source = support::get<T>(); std::cerr << "source=" << source << std::endl;
        T       target = 0;
        loc.copy(&target,&source);
        Y_CHECK(target==source);
        Y_CHECK(sizeof(T)==loc.save(fp, &source));
        Y_CHECK(sizeof(T)==nbo.save(fp, &source));
        for(size_t i=1;i<=fp.size();++i)
        {
            std::cerr << ' ' << hexadecimal::lowercase[ fp[i] ];
        }
        std::cerr << std::endl;
        
        {
            ios::imstream in(fp);
            target = 0;
            Y_CHECK(sizeof(T)==loc.load(&target,in));
            Y_CHECK(target==source);

            target = 0;
            Y_CHECK(sizeof(T)==nbo.load(&target,in));
            Y_CHECK(target==source);
        }
        
    }
}

Y_UTEST(ios_carrier)
{
    
    test_plain<int>();

}
Y_UTEST_DONE()


