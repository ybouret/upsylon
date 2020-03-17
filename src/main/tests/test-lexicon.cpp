#include "y/associative/map.hpp"
#include "y/associative/suffix-table.hpp"
#include "y/sequence/vector.hpp"
#include "y/string/convert.hpp"
#include "y/type/spec.hpp"
#include "y/utest/run.hpp"
#include "y/utest/timings.hpp"

using namespace upsylon;

namespace {
    
    template <typename KEY, typename T>
    void doLexicon(lexicon<KEY,T>        &L,
                   const accessible<KEY> &keys,
                   const accessible<T>   &args,
                   const char            *usr)
    {
        assert(usr);
        std::cerr << "lexicon<" << type_name_of<KEY>() << "," << type_name_of<T>() << ">=" << usr << std::endl;
        // check
        Y_ASSERT(keys.size()==args.size());
        const size_t n = keys.size();
        
        // insert and track all keys
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
        
        // cleanup
        while( ok.size() > n/2 )
        {
            const KEY &bad = ok.back();
            Y_ASSERT( L.remove(bad) );
            Y_ASSERT( NULL == L.search(bad) );
            no << bad;
            ok.pop_back();
        }
        
        Y_CHECK(ok.size()+no.size()==n);
        
        const double D = 0.5;
        
        std::cerr << "\tsearching existing keys...";
        
        {
            double ok_speed = 0;
            Y_TIMINGS(ok_speed,D,
                      for(size_t i=ok.size();i>0;--i)
                      Y_ASSERT( NULL != L.search(ok[i]) );
                      );
            ok_speed *= ok.size();
            ok_speed /= 1e6;
            std::cerr << " @" << ok_speed << " Mops" << std::endl;
        }
        
        {
            std::cerr << "\tsearching removed  keys...";
            double no_speed = 0;
            Y_TIMINGS(no_speed,D,
                      for(size_t i=no.size();i>0;--i)
                      Y_ASSERT( NULL == L.search(no[i]) );
                      );
            no_speed *= no.size();
            no_speed /= 1e6;
            std::cerr << " @" << no_speed << " Mops" << std::endl;
        }
        
        std::cerr << std::endl;
    }
    
}


Y_UTEST(lexicon)
{
    size_t n = 32;
    if(argc>1)
    {
        n = string_convert::to<size_t>(argv[1],"n");
    }
    std::cerr << "generating " << n << " words..." << std::endl;
    vector<string> strings(n,as_capacity);
    for(size_t i=1;i<=n;++i)
    {
        while(true)
        {
            size_t m=2+alea.leq(8);
            string s;
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
    map<string,string>           smap(n,as_capacity);
    suffix_table<string,string>  stab(n,as_capacity);
    
    doLexicon(smap,strings,strings,"map");
    doLexicon(stab,strings,strings,"suffix_table");
    
    
    
}
Y_UTEST_DONE()


