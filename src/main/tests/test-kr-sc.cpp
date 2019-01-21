#include "y/crypto/sc/arc4.hpp"
#include "y/crypto/sc/isaac-ks.hpp"
#include "y/string.hpp"

#include "y/sequence/vector.hpp"

#include "y/utest/run.hpp"

using namespace upsylon;

namespace
{
    void do_test_sc( crypto::kstream &ks, const memory::ro_buffer &key)
    {
        //ks.schedule(key);

        vector<char> source(32,as_capacity);
        vector<char> target(source.capacity(),as_capacity);
        for(size_t i=32;i>0;--i)
        {
            const char C = alea.range<char>('a','z');
            source.push_back(C);
            target.push_back( ks(C) );
        }

        vector<char> decoded(target.size());
        ks.schedule(key);
        ks(*decoded,*target,target.size());
        std::cerr << "source  = " << source  << std::endl;
        std::cerr << "decoded = " << decoded << std::endl;
        for(size_t i=target.size();i>0;--i)
        {
            Y_ASSERT(source[i]==decoded[i]);
        }

    }
}

Y_UTEST(kr_sc)
{

    for(size_t iter=8;iter>0;--iter)
    {
        string key;
        for(size_t i=1+alea.leq(32);i>0;--i)
        {
            key += alea.full<char>();
        }

        crypto::arc4     sc_arc4(key);
        crypto::isaac_ks sc_iks(key);

        do_test_sc(sc_arc4,key);
        do_test_sc(sc_iks,key);
    }


}
Y_UTEST_DONE()

