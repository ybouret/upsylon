#include "y/ios/carrier/primary.hpp"
#include "y/ios/carrier/network.hpp"
#include "y/ios/carrier/derived.hpp"
#include "y/ios/carrier/tuple.hpp"

#include "y/ios/ovstream.hpp"
#include "y/ios/imstream.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

#include "support.hpp"
#include "y/code/utils.hpp"

using namespace upsylon;

namespace
{
    
    template <typename T>
    void do_test(const ios::carrier &io)
    {
        std::cerr << "Testing " << io << std::endl;
        ios::ovstream   fp;
        const size_t    n = 10 + alea.leq(10);
        vector<T>       source(n,as_capacity);
        vector<T>       target(n,as_capacity);
        
        for(size_t i=n;i>0;--i)
        {
            {
                const T tmp = support::get<T>();
                source.push_back_(tmp);
            }
            
            {
                const T tmp = support::get<T>();
                target.push_back_(tmp);
            }
            
        }
        
        size_t sum = 0;
        for(size_t i=1;i<=n;++i)
        {
            Y_ASSERT( sizeof(T) == io.copy(&target[i],&source[i]) );
            Y_ASSERT( target[i] == source[i]);
            sum += io.save(fp,&source[i]);
        }
        Y_ASSERT(sum==fp.size());
        
        sum = 0;
        {
            ios::imstream in(fp);
            for(size_t i=1;i<=n;++i)
            {
                {
                    const T tmp = support::get<T>();
                    target[i] = tmp;
                }
                
                sum += io.load(&target[i],in);
                Y_ASSERT(target[i]==source[i]);
            }
        }
        Y_ASSERT(fp.size()==sum);
        
    }
    
    template <typename T>
    void test_plain()
    {
        {
            ios::primary_carrier<T> io;
            do_test<T>(io);
        }
        
        {
            ios::network_carrier<T> io;
            do_test<T>(io);
        }
    }
    
    template <typename T>
    void test_class()
    {
        ios::derived_carrier<T> io;
        do_test<T>(io);
    }
    
    
    template <
    template <typename> class TUPLE,
    typename                  T,
    template <typename> class CONVEYOR
    >
    static inline void test_tuple()
    {
        ios::tuple_carrier<TUPLE,T,CONVEYOR> io;
        std::cerr << io << std::endl;
        
        
        
        Y_UTEST_SIZEOF(io);
    }
    
}

Y_UTEST(ios_carrier)
{
    
    test_plain<uint8_t>();
    test_plain<int16_t>();
    test_plain<uint32_t>();
    test_plain<int64_t>();
    
    test_class<string>();
    test_class<apq>();
    
    test_tuple<complex,float,ios::primary_carrier>();
    test_tuple<complex,float,ios::network_carrier>();
    test_tuple<point2d,float,ios::network_carrier>();
    test_tuple<point3d,double,ios::primary_carrier>();

    
    
    Y_UTEST_SIZEOF(ios::primary_carrier<uint8_t>);
    Y_UTEST_SIZEOF(ios::network_carrier<uint8_t>);

    Y_UTEST_SIZEOF(ios::primary_carrier<uint64_t>);
    Y_UTEST_SIZEOF(ios::network_carrier<uint64_t>);
    
    Y_UTEST_SIZEOF(ios::derived_carrier<string>);
    Y_UTEST_SIZEOF(ios::derived_carrier<apn>);
    Y_UTEST_SIZEOF(ios::derived_carrier<apq>);

    
}
Y_UTEST_DONE()


