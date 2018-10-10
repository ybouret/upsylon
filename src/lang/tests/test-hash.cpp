#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/ios/icstream.hpp"
#include "y/lang/dynamo.hpp"
#include "y/hashing/elf.hpp"
#include "y/hashing/fnv.hpp"
#include "y/hashing/pjw.hpp"
#include "y/hashing/sfh.hpp"

using namespace upsylon;
using namespace Lang;

template <typename HF>
static inline void check( const array<string> &words )
{
    DynamoHasherWith<HF> H;
    std::cerr << "checking with " << H.id << std::endl;
    const size_t n = words.size();
    vector<int32_t> hashs(n,0);
    for(size_t i=n;i>0;--i)
    {
        hashs[i] = H(words[i]);
    }
    size_t collisions = 0;
    for(size_t i=1;i<n;++i)
    {
        const int32_t h_i = hashs[i];
        for(size_t j=i+1;j<=n;++j)
        {
            const int32_t h_j = hashs[j];
            if(h_i==h_j)
            {
                std::cerr << ":" << words[i] << "/" << words[j];
                ++collisions;
            }
        }
    }
    if(collisions)
        std::cerr << std::endl;
    std::cerr << H.id << ": " << collisions << std::endl;

}

Y_UTEST(hash)
{
    vector<string>   words;
    vector<int32_t>  hashs;


    {
        ios::icstream fp( ios::cstdin );
        string line;
        while( (std::cerr << "> ").flush(), fp.gets(line) )
        {
            words << line;
        }
        std::cerr << std::endl;
    }

    check<hashing::sha1>(words);
    check<hashing::elf>(words);
    check<hashing::fnv>(words);
    check<hashing::sfh>(words);
    check<hashing::pjw>(words);



}
Y_UTEST_DONE()
