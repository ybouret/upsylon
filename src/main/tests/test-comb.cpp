#include "y/counting/comb.hpp"
#include "y/utest/run.hpp"
#include "y/string/convert.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;


Y_UTEST(comb)
{
    
    int n = 5; /* The size of the set; for {1, 2, 3, 4} it's 4 */
    int k = 3; /* The size of the subsets; for {1, 2}, {1, 3}, ... it's 2 */
   
    
    if(argc>1) n = string_convert::to<unit_t>(argv[1]); Y_ASSERT(n>0);
    if(argc>2) k = string_convert::to<unit_t>(argv[2]); Y_ASSERT(k>0); Y_ASSERT(k<=n);
    

    combination comb(n,k);

    std::cerr << "comb(" << n << "," << k << ")=" << comb.count << std::endl;

    vector<combination> Comb(comb.count,as_capacity);

    for( comb.start(); comb.active(); comb.next() )
    {
        std::cerr << "\t" << comb << std::endl;
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
Y_UTEST_DONE()


