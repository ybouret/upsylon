#include "y/associative/map.hpp"
#include "y/associative/suffix-table.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/string/convert.hpp"
using namespace upsylon;

namespace {
 
    template <typename KEY, typename T>
    void doLexicon(lexicon<KEY,T>        &L,
                   const accessible<KEY> &keys,
                   const accessible<T>   &args)
    {
        Y_ASSERT(keys.size()==args.size());
        const size_t n = keys.size();
        
        vector<KEY> ok(n,as_capacity);
        vector<KEY> no(n,as_capacity);
        
        for(size_t i=1;i<=n;++i)
        {
            const KEY &key = keys[i];
            if( L.insert(key,args[i]) )
            {
                ok << key;
            }
            else
            {
                no << key;
            }
        }
        Y_CHECK(L.size()==ok.size());
        while( ok.size() > n/2 )
        {
            const KEY &bad = ok.back();
            Y_ASSERT( L.remove(bad) );
            Y_ASSERT( NULL == L.search(bad) );
            no << bad;
            ok.pop_back();
        }
    }
    
}


Y_UTEST(lexicon)
{
    size_t n = 100;
    if(argc>1)
    {
        n = string_convert::to<size_t>(argv[1],"n");
    }
    vector<string> strings(n,as_capacity);
    for(size_t i=1;i<=n;++i)
    {
        while(true)
        {
            size_t m=1+alea.leq(7);
            string s(m,as_capacity,false);
            while(m-- > 0 ) s << alea.range<char>(33,126);
            bool found = false;
            for(size_t j=1;j<i;++j)
            {
                if( strings[j] == s )
                {
                    found = true;
                    break;
                }
            }
            if( found )
            {
                continue;
            }
            else
            {
                strings << s;
                break;
            }
        }
    }
    //std::cerr << "strings=" << strings << std::endl;
    map<string,string>           smap(n,as_capacity);
    suffix_table<string,string>  stab(n,as_capacity);
    
    doLexicon(smap,strings,strings);
    doLexicon(stab,strings,strings);

    
    
}
Y_UTEST_DONE()


