#include "y/counting/perm.hpp"
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

        permutation        perm(num);
        suffix_store<char> keys;
        
        size_t count = 0;
        size_t n_out = 0;
        for( perm.boot(); perm.good(); perm.next() )
        {
            perm.apply(*ana, *org);
            if( keys.insert(*ana,num) )
            {
                ++n_out;
                std::cerr << "<" << ana << ">" << std::endl;
            }
            ++count;
        }
        Y_CHECK(perm.count==count);
        std::cerr << "[" << n_out << "/" << count << "]" << std::endl;
       
    }
    
}
Y_UTEST_DONE()
