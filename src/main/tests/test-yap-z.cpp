
#include "y/yap/integer.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/icstream.hpp"


using namespace upsylon;
using namespace yap;

#define ITER 128

namespace {

}

Y_UTEST(yap_z)
{

    vector<integer> Z;
    {
        const integer zero;         std::cerr << "zero  = "  << zero  << std::endl;
        const integer one = 1;      std::cerr << "one   = "  << one   << std::endl;
        const integer minus = -1;   std::cerr << "minus = "  << minus << std::endl;
        Z.push_back(zero);
        Z.push_back(one);
        Z.push_back(minus);
        integer tmp;
        tmp = one; Z.push_back(tmp);
        tmp = 5;   Z.push_back(tmp);
        tmp = -7;  Z.push_back(tmp);
    }
    std::cerr << Z << std::endl;
    for(size_t i=0;i<ITER;++i)
    {
        const integer z(alea,alea.leq(80));
        Z.push_back(z);
    }

    {
        size_t written = 0;
        {
            ios::ocstream fp("apz.dat");
            for(size_t i=1;i<=Z.size();++i)
            {
                written += Z[i].serialize(fp);
            }
        }
        std::cerr << "written=" << written << std::endl;
        size_t read = 0;
        {
            ios::icstream fp("apz.dat");
            for(size_t i=1;i<=Z.size();++i)
            {
                size_t delta = 0;
                const integer z = integer::read(fp, delta, "some integer");
                read += delta;
                if(z!=Z[i])
                {
                    std::cerr << "read " << z << " for " << Z[i] << std::endl;
                    throw exception("bad integer read");
                }
                Y_ASSERT(z==Z[i]);
                Y_ASSERT(!(z!=Z[i]));
            }
        }
        Y_ASSERT(read==written);
    }

    std::cerr << std::endl;
    std::cerr << "Memory Usage:" << std::endl;
    std::cerr << *natural::instance() << std::endl;

}
Y_UTEST_DONE()


