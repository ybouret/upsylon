#include "y/hashing/adler32.hpp"
#include "y/hashing/bjh32.hpp"
#include "y/hashing/crc16.hpp"
#include "y/hashing/crc32.hpp"
#include "y/hashing/elf.hpp"
#include "y/hashing/fnv.hpp"
#include "y/hashing/pjw.hpp"
#include "y/hashing/sfh.hpp"
#include "y/hashing/sha1.hpp"

#include "y/hashing/md2.hpp"
#include "y/hashing/md4.hpp"
#include "y/hashing/md5.hpp"
#include "y/hashing/rmd128.hpp"
#include "y/hashing/rmd160.hpp"
#include "y/hashing/sha256.hpp"
#include "y/hashing/sha512.hpp"


#include "y/utest/run.hpp"

#include "y/sequence/vector.hpp"
#include "y/ios/icstream.hpp"
#include "y/utest/timings.hpp"
#include "y/hashing/key-hasher.hpp"

#include "y/string/display.hpp"

using namespace upsylon;

namespace
{
    template <typename KEY>
    static inline
    void collide( hashing::function &H, const accessible<string> &arr )
    {
        string_display::align(std::cerr << "Collisions in ",H.name(),12)<< "...";
        vector<KEY> K( arr.size(), as_capacity );
        for(size_t i=arr.size();i>0;--i)
        {
            K.push_back_( H.key<KEY>(arr[i]) );
        }
        std::cerr << "...";
        size_t ans = 0;
        for(size_t i=arr.size();i>0;--i)
        {
            const KEY &KI = K[i];
            for(size_t j=i-1;j>0;--j)
            {
                const KEY &KJ = K[j];
                if( KI == KJ )
                {
                    ++ans;
                    //std::cerr << I << "<=>" << J << std::endl;
                }
            }
        }
        std::cerr << ans << std::endl;
    }

    static inline
    double perform( hashing::function &H, const array<string> &arr )
    {
        string_display::align(std::cerr <<"\t",H.name(),20) << " @ ";
        double speed = 0;
        Y_TIMINGS(speed,1,for(size_t i=arr.size();i>0;--i) (void) H.key<uint32_t>(arr[i]));
        speed *= arr.size() / 1e6;
        std::cerr << speed << " Mops/s" << std::endl;
        return speed;
    }

}

#include "y/sort/heap.hpp"

Y_UTEST(hashing)
{
    vector< hashing::function::pointer > phash;
#define __REGISTER(CLASS) \
do { hashing::function::pointer p = new hashing:: CLASS(); phash.push_back(p); \
std::cerr << "sizeof(" #CLASS ")=" << sizeof(hashing:: CLASS)<< std::endl;     \
std::cerr << "sizeof(key_hasher." #CLASS ")=" << sizeof(key_hasher<string,hashing::CLASS>)<< std::endl;     \
} while(false)

    __REGISTER(adler32);
    __REGISTER(bjh32);
    __REGISTER(crc16);
    __REGISTER(crc32);
    __REGISTER(elf);
    __REGISTER(fnv);
    __REGISTER(pjw);
    __REGISTER(sfh);
    __REGISTER(sha1);
    __REGISTER(md2);
    __REGISTER(md4);
    __REGISTER(md5);
    __REGISTER(rmd128);
    __REGISTER(rmd160);
    __REGISTER(sha224);
    __REGISTER(sha256);
    __REGISTER(sha384);
    __REGISTER(sha512);

    std::cerr << "Hashing Functions: " << std::endl;
    for(size_t i=1;i<=phash.size();++i)
    {
        std::cerr << "\t" << phash[i]->name() << std::endl;
    }
    const string s = "Hello, World";

    {
        std::cerr << "Testing Key Hasher: " << std::endl;
        key_hasher<string>               skh_default;
        key_hasher<string,hashing::sha1> skh_crypto;
        std::cerr << "\tdefault=" << skh_default(s) << std::endl;
        std::cerr << "\tcrypto =" << skh_crypto(s)  << std::endl;
    }

    std::cerr << "Testing Message Digest" << std::endl;
    digest d(8);
    for(size_t i=1;i<=phash.size();++i)
    {
        hashing::function &H = *phash[i];
        H.set();
        H(s);
        H.out(d);

        H.set();
        H(s);
        const digest D = H.md();
        string_display::align( std::cerr << "\t" << d << "@", H.name(),8) << " => " << D << std::endl;
    }

    if(argc>1)
    {
        vector<string> lines;
        string line;
        ios::icstream fp(argv[1]);
        while( fp.gets(line) )
        {
            lines.push_back(line);
        }
        const size_t words=lines.size();
        std::cerr << "#words=" << words << std::endl;
        std::cerr << "Collisions:" << std::endl;
        for(size_t i=1;i<=phash.size();++i)
        {
            collide<uint32_t>(*phash[i],lines);
        }
        std::cerr << "Performances:" << std::endl;
        vector<string> names;
        vector<double> speeds;

        for(size_t i=1;i<=phash.size();++i)
        {
            hashing::function &h = *phash[i];
            names .push_back( h.name() );
            speeds.push_back( perform(h,lines) );
        }
        std::cerr << "Comparisons: " << std::endl;
        hsort(speeds, names, comparison::decreasing<double> );
        for(size_t i=1;i<=names.size();++i)
        {
            string_display::align( std::cerr << "\t", names[i], 12 ) << " @ " << speeds[i] << " Mops" << std::endl;
        }


    }

}
Y_UTEST_DONE()



