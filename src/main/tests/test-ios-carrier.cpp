#include "y/ios/carrier/primary.hpp"
#include "y/ios/carrier/network.hpp"
#include "y/ios/carrier/derived.hpp"
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
    
    template <typename T>
    void test_class()
    {
        ios::derived_carrier<T> carry;
        ios::ovstream           fp;
        
        T       source = support::get<T>(); std::cerr << "source=" << source << std::endl;
        T       target;
        carry.copy(&target,&source);
        Y_CHECK(target==source);
        
        source = support::get<T>(); std::cerr << "source=" << source << std::endl;
        const size_t nw = carry.save(fp,&source);
        Y_CHECK(nw == fp.size() );
        for(size_t i=1;i<=fp.size();++i)
        {
            std::cerr << ' ' << hexadecimal::lowercase[ fp[i] ];
        }
        std::cerr << std::endl;
        {
            ios::imstream in(fp);
            const size_t nr = carry.load(&target,in);
            Y_CHECK(fp.size()==nr);
            Y_CHECK(target==source);
        }
        
        
    }
    
}

Y_UTEST(ios_carrier)
{
    
    test_plain<uint8_t>();
    test_plain<int16_t>();
    test_plain<uint32_t>();
    test_plain<uint64_t>();

    test_class<string>();
    test_class<apq>();

}
Y_UTEST_DONE()


