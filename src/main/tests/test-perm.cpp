
#include "y/counting/permuter.hpp"
#include "y/counting/perm.hpp"
#include "y/utest/run.hpp"
#include "y/string/convert.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;


Y_UTEST(perm)
{
    for(int iarg=1;iarg<argc;++iarg)
    {
        const size_t n =  string_convert::to<size_t>(argv[iarg]);

        permutation         perm(n);
        vector<permutation> Perm( perm.count, as_capacity );

        std::cerr << "#perm(" << n << ")=" << perm.count << std::endl;
        for( perm.start(); perm.valid(); perm.next() )
        {
            std::cerr << "\t" << perm;
            std::cerr << " (";
            for(size_t i=0;i<n;++i) std::cerr << ' ' << perm(i) ;
            std::cerr << " )" << std::endl;
            Perm.push_back_(perm);
        }
        std::cerr << "#perm(" << n << ")=" << perm.count << std::endl;
        std::cerr << "checking..." << std::endl;
        for(size_t i=1;i<=Perm.size();++i)
        {
            for(size_t j=i;j<=Perm.size();++j)
            {
                permutation::memchk(Perm[i],Perm[j]);
            }
        }
    }

}
Y_UTEST_DONE()

#include "y/sort/unique.hpp"
Y_UTEST(anagram)
{
    if(argc>1)
    {
        const string org = argv[1];
        const size_t num = org.length();
        permutation  perm(num);
        vector<string> all( perm.count, as_capacity );
        for( perm.start(); perm.valid(); perm.next() )
        {
            string ana;
            for(size_t i=0;i<perm.n;++i)
            {
                ana << org[ perm(i) ];
            }
            all << ana;
        }
        unique(all);
        for(size_t i=1;i<=all.size();++i)
        {
            std::cerr << all[i] << std::endl;
        }
    }

}
Y_UTEST_DONE()

