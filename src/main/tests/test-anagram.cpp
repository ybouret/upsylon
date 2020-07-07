#include "y/counting/perm.hpp"
#include "y/counting/permuter.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"
#include "y/associative/suffix-store.hpp"

using namespace upsylon;

Y_UTEST(anagram)
{
    if(argc>1)
    {
        const string       org = argv[1];
        const size_t       num = org.length();
        string             ana(num,as_capacity,true);
        permuter<char>     perm(*org,num);
        permutation        theo(num);

        std::cerr << "#perm = " << perm.count << " / #theo = " << theo.count << std::endl;
        for( perm.boot(); perm.good(); perm.next() )
        {
            perm.apply(*ana);
            std::cerr << "<" << ana << ">" << std::endl;
        }

    }

}
Y_UTEST_DONE()
