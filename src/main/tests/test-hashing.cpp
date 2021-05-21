

#include "y/utest/run.hpp"
#include "y/hashing/sha1.hpp"

#include "y/sequence/vector.hpp"
#include "y/ios/icstream.hpp"
#include "y/utest/timings.hpp"
#include "y/hashing/key-hasher.hpp"
#include "y/string/display.hpp"

#include "y/hashing/factory.hpp"

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
    hashing::factory                    &hf = hashing::factory::instance();
    vector< hashing::function::pointer > phash(hf.size(),as_capacity);
    for( hashing::factory::iterator i = hf.begin(); i != hf.end(); ++i )
    {
        std::cerr << "[" << i.get().key() << "]" << std::endl;
        const hashing::function::pointer p = hf( i.get().key() );
        phash.push_back(p);
    }

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



