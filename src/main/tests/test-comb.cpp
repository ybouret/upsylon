#include "y/counting/comb.hpp"
#include "y/utest/run.hpp"
#include "y/string/convert.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;


Y_UTEST(comb)
{

    for(int iarg=1;iarg<argc;++iarg)
    {
        const size_t n = string_convert::to<size_t>(argv[1],"n");

        for(size_t k=1;k<=n;++k)
        {
            combination comb(n,k);
            std::cerr << "comb(" << n << "," << k << ")=" << comb.count << std::endl;

            vector<combination> Comb(comb.count,as_capacity);

            for( comb.start(); comb.valid(); comb.next() )
            {
                std::cerr << "\t" << comb;
                std::cerr << " (";
                for(size_t i=0;i<k;++i)
                {
                    std::cerr << ' ' << comb(i);
                }
                std::cerr << " )";
                std::cerr << std::endl;
                Comb.push_back_(comb);
            }
            std::cerr << "comb(" << n << "," << k << ")=" << comb.count << std::endl;
            std::cerr << "checking..." << std::endl;
            for(size_t i=1;i<=Comb.size();++i)
            {
                for(size_t j=i;j<=Comb.size();++j)
                {
                    combination::memchk(Comb[i],Comb[j]);
                }
            }
        }
    }
}
Y_UTEST_DONE()


