#include "y/hashing/imph.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/ios/icstream.hpp"
#include <typeinfo>

using namespace upsylon;

namespace
{
    template <typename T> static inline
    void doMPH()
    {
        std::cerr << "mperf_for<" << typeid(T).name() << ">" << std::endl;
        hashing::mperf_for<T> H(1);

        vector<T> keys;
        for(size_t i=1024;i>0;--i)
        {
            const T key = alea.partial<T>();
            if( H.insert(key) )
            {
                keys.push_back(key);
            }
        }
        std::cerr << "|_#keys=" << keys.size() << std::endl;
        //std::cerr << keys << std::endl;
        for(size_t i=keys.size();i>0;--i)
        {
            Y_ASSERT( i == H[ keys[i] ] );
        }

    }
}

Y_UTEST(mperf)
{
    hashing::mperf H;

    if( argc>1 )
    {
        ios::icstream fp(argv[1]);
        string line;
        int    indx = 0;
        while( fp.gets(line) )
        {
            H.insert(line,indx++);
        }
    }

    H.optimize();
    std::cerr << "#nodes=" << H.nodes << std::endl;

    doMPH<uint8_t>();
    doMPH<uint16_t>();
    doMPH<uint32_t>();
    doMPH<uint64_t>();

    doMPH<int8_t>();
    doMPH<int16_t>();
    doMPH<int32_t>();
    doMPH<int64_t>();

    doMPH<int>();


}
Y_UTEST_DONE()

