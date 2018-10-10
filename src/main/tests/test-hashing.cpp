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

using namespace upsylon;

namespace
{
    template <typename KEY>
    static inline
    void collide( hashing::function &H, const array<string> &arr )
    {
        std::cerr << "Collisions in " << H.name() << "..." << std::endl;
        size_t ans = 0;
        for(size_t i=arr.size();i>0;--i)
        {
            const string &I = arr[i];
            const KEY     KI = H.key<KEY>(I);
            for(size_t j=i-1;j>0;--j)
            {
                const string &J = arr[j];
                const KEY KJ = H.key<KEY>(J);
                if( KI == KJ )
                {
                    ++ans;
                    //std::cerr << I << "<=>" << J << std::endl;
                }
            }
        }
        std::cerr << "\t\t" << ans << std::endl;
    }

    static inline
    void perform( hashing::function &H, const array<string> &arr )
    {
        std::cerr << H.name() << std::endl;
        double speed = 0;
        Y_TIMINGS(speed,1,for(size_t i=arr.size();i>0;--i) (void) H.key<uint32_t>(arr[i]));
        std::cerr << "\t|_speed=" << speed << std::endl;
    }

}

Y_UTEST(hashing)
{
    vector< hashing::function::pointer > phash;
#define __REGISTER(CLASS) do { hashing::function::pointer p = new hashing:: CLASS(); phash.push_back(p); } while(false)
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

    for(size_t i=1;i<=phash.size();++i)
    {
        std::cerr << phash[i]->name() << std::endl;
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
        for(size_t i=1;i<=phash.size();++i)
        {
            collide<uint32_t>(*phash[i],lines);
        }
        for(size_t i=1;i<=phash.size();++i)
        {
            perform(*phash[i],lines);
        }
    }

    key_hasher<string>               skh_default;
    key_hasher<string,hashing::sha1> skh_crypto;
    const string s = "Hello, World!";
    std::cerr << "default=" << skh_default(s) << std::endl;
    std::cerr << "crypto =" << skh_crypto(s)  << std::endl;

}
Y_UTEST_DONE()

