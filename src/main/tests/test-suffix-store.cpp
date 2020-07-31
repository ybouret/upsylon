#include "y/associative/suffix/store.hpp"
#include "y/associative/suffix/xstore.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/string.hpp"
#include "y/sequence/vector.hpp"
#include "y/type/spec.hpp"

using namespace upsylon;

namespace {
    template <typename STORE>
    static inline void copyStore( const STORE &source )
    {
        typedef typename STORE::type type;
        const string &name = type_name_of<type>();
        std::cerr << "with type=<" << name << "> and #nodes=" << source.nodes << std::endl;
        STORE target(source);
        Y_CHECK(target.nodes==source.nodes);
        Y_CHECK(target.is_same_than(source));
        const string fn = "store_"+name+".dat";
        size_t       nw = 0;
        {
            ios::ocstream fp(fn);
            nw = source.serialize(fp);
            std::cerr << "\t#written in " << fn << " : " << nw << std::endl;

        }
        {
            ios::icstream fp(fn);
            const size_t nr = target.load(fp);
            std::cerr << "\t#read from  " << fn  << " : " << nr << std::endl;
            Y_CHECK(nw==nr);
            Y_CHECK(target.nodes==source.nodes);
            Y_CHECK(target.is_same_than(source));
        }

    }
}

Y_UTEST(store)
{
    if(true)
    {
        Y_UTEST_SIZEOF( suffix_store<uint8_t>::node_type  );
        Y_UTEST_SIZEOF( suffix_store<uint16_t>::node_type );
        Y_UTEST_SIZEOF( suffix_store<uint32_t>::node_type );
        Y_UTEST_SIZEOF( suffix_store<uint64_t>::node_type );
        std::cerr << std::endl;
        Y_UTEST_SIZEOF( suffix_xstore<uint8_t>::node_type  );
        Y_UTEST_SIZEOF( suffix_xstore<uint16_t>::node_type );
        Y_UTEST_SIZEOF( suffix_xstore<uint32_t>::node_type );
        Y_UTEST_SIZEOF( suffix_xstore<uint64_t>::node_type );
    }
    
    suffix_store<char>     s8;  suffix_xstore<char>     x8;
    suffix_store<uint16_t> s16; suffix_xstore<uint16_t> x16;
    suffix_store<uint32_t> s32; suffix_xstore<uint32_t> x32;
    suffix_store<uint64_t> s64; suffix_xstore<uint64_t> x64;
    
    vector<string>     keys;
    size_t             total=0;
    if(argc>1)
    {
        std::cerr << "-- loading keys" << std::endl;
        {
            const string  fileName = argv[1];
            ios::icstream fp( fileName);
            string        line;
            
            while(fp.gets(line))
            {
                if(s8.insert( *line, line.size() ))
                {
                    keys << line;
                    std::cerr << "+ '" << line << "'" << std::endl;
                    total += line.size();
                    Y_ASSERT( !s8.insert(*line,line.size() ) );
                    Y_ASSERT(s16.insert( *line, line.size() )); Y_ASSERT(!s16.insert( *line, line.size() ));
                    Y_ASSERT(s32.insert( *line, line.size() )); Y_ASSERT(!s32.insert( *line, line.size() ));
                    Y_ASSERT(s64.insert( *line, line.size() )); Y_ASSERT(!s64.insert( *line, line.size() ));
                    
                    Y_ASSERT(x8.insert( *line, line.size() ) ); Y_ASSERT(!x8.insert(  *line, line.size() ));
                    Y_ASSERT(x16.insert(*line, line.size() ) ); Y_ASSERT(!x16.insert( *line, line.size() ));
                    Y_ASSERT(x32.insert(*line, line.size() ) ); Y_ASSERT(!x32.insert( *line, line.size() ));
                    Y_ASSERT(x64.insert(*line, line.size() ) ); Y_ASSERT(!x64.insert( *line, line.size() ));
                }
            }
        }
        
        std::cerr << "-- checking keys" << std::endl;
        alea.shuffle( *keys, keys.size() );
        for(size_t i=keys.size();i>0;--i)
        {
            const string &key = keys[i];
            const char   *k   = *key;
            const size_t  n   = key.size();
            Y_ASSERT( s8.has(k,n)  ); Y_ASSERT( x8.has(k,n)  );
            Y_ASSERT( s16.has(k,n) ); Y_ASSERT( x16.has(k,n) );
            Y_ASSERT( s32.has(k,n) ); Y_ASSERT( x32.has(k,n) );
            Y_ASSERT( s64.has(k,n) ); Y_ASSERT( x64.has(k,n) );
        }
        
    }


    
    std::cerr << "-- memory stats" << std::endl;
    std::cerr << "#bytes  = " << total    << std::endl;
    
    std::cerr << "#nodes  = " << s8.nodes  << " -> " <<  s8.nodes * sizeof(suffix_store<uint8_t>::node_type)  << std::endl;
    std::cerr << "        = " << s16.nodes << " -> " << s16.nodes * sizeof(suffix_store<uint16_t>::node_type) <<std::endl;
    std::cerr << "        = " << s32.nodes << " -> " << s32.nodes * sizeof(suffix_store<uint32_t>::node_type) <<std::endl;
    std::cerr << "        = " << s64.nodes << " -> " << s64.nodes * sizeof(suffix_store<uint64_t>::node_type) <<std::endl;
    
    std::cerr << "#xnodes = " << x8.nodes  << " -> " <<  x8.nodes * sizeof(suffix_xstore<uint8_t>::node_type)  << std::endl;
    std::cerr << "        = " << x16.nodes << " -> " << x16.nodes * sizeof(suffix_xstore<uint16_t>::node_type) <<std::endl;
    std::cerr << "        = " << x32.nodes << " -> " << x32.nodes * sizeof(suffix_xstore<uint32_t>::node_type) <<std::endl;
    std::cerr << "        = " << x64.nodes << " -> " << x64.nodes * sizeof(suffix_xstore<uint64_t>::node_type) <<std::endl;

    copyStore(s8);
    copyStore(s16);
    copyStore(s32);
    copyStore(s64);

    
}
Y_UTEST_DONE()

#include "y/type/spec.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/utest/timings.hpp"

namespace {
    
    
    typedef vector<uint8_t,memory::pooled> key_type;
    
    template <
    typename STYPE,
    typename XTYPE>
    void checkPerf(const STYPE &s,
                   const XTYPE &x,
                   const vector<key_type> &keys  )
    {
        const double D  = 1;
        const size_t nk = keys.size();
        {
            double s_speed = 0;
            Y_TIMINGS(s_speed,
                      D,
                      for(size_t i=nk;i>0;--i)
                      { const key_type &key = keys[i]; Y_ASSERT(s.has(*key,key.size())); }
                      );
            s_speed *= nk;
            s_speed /= 1e6;
            std::cerr << "\ts_speed.ok=" << s_speed << " Mops" << std::endl;
        }
        
        {
            double x_speed = 0;
            Y_TIMINGS(x_speed,
                      D,
                      for(size_t i=nk;i>0;--i)
                      { const key_type &key = keys[i]; Y_ASSERT(x.has(*key,key.size())); }
                      );
            x_speed *= nk;
            x_speed /= 1e6;
            std::cerr << "\tx_speed.ok=" << x_speed << " Mops" << std::endl;
        }
        std::cerr << std::endl;
    }
    
    
    
}

#include "y/string/convert.hpp"
#include "y/string/tokenizer.hpp"
Y_UTEST(store_perf)
{
    //! maximal number of keys
    size_t n = 2;
    if(argc>1)
    {
        n = string_convert::to<size_t>(argv[1],"n");
    }

    //! key length+range
    const char *ks = "255";
    if( argc > 2 ) ks = argv[2];

    vector<uint8_t> r;
    {
        const string   rs = ks;
        vector<string> words;
        tokenizer<char>::split_with(words, rs, ':');
        Y_ASSERT( words.size()>0 );
        r.ensure(words.size());
        for(size_t i=1;i<=words.size();++i)
        {
            const size_t ri = string_convert::to<size_t>( words[i], "range" );
            Y_ASSERT(ri<256);
            r.push_back( uint8_t(ri) );
        }
    }
    std::cerr << "#r=" << r.size() << std::endl;
    

    vector<key_type>                       ok(n,as_capacity);
    vector<key_type>                       no;
    
    suffix_store<uint8_t>   s8;  suffix_xstore<uint8_t> x8;
    suffix_store<uint16_t>  s16; suffix_xstore<uint16_t> x16;
    suffix_store<uint32_t>  s32; suffix_xstore<uint32_t> x32;
    suffix_store<uint64_t>  s64; suffix_xstore<uint64_t> x64;

    
    for(size_t i=1;i<=n;++i)
    {
        for(size_t ntry=0;ntry<16;++ntry)
        {
            const size_t num = r.size();
            key_type     key(num,0);
            Y_ASSERT(num==key.size());
            for(size_t j=1;j<=num;++j)
            {
                key[j] = alea.range<uint8_t>(0,r[j]);
            }
            
            const uint8_t *k = *key;
            const size_t   l = key.size();
            if( s8.insert(k,l) )
            {
                Y_ASSERT( s8.has(k,l) );
                Y_ASSERT( x8.insert(k,l) ); Y_ASSERT( x8.has(k,l) );
                
                Y_ASSERT( s16.insert(k,l) ); Y_ASSERT( s16.has(k,l) );
                Y_ASSERT( x16.insert(k,l) ); Y_ASSERT( x16.has(k,l) );

                Y_ASSERT( s32.insert(k,l) ); Y_ASSERT( s32.has(k,l) );
                Y_ASSERT( x32.insert(k,l) ); Y_ASSERT( x32.has(k,l) );
                
                Y_ASSERT( s64.insert(k,l) ); Y_ASSERT( s64.has(k,l) );
                Y_ASSERT( x64.insert(k,l) ); Y_ASSERT( x64.has(k,l) );
                
                ok << key;
                break;
            }
        }
        //std::cerr << ok.back() << std::endl;
    }
    const size_t nk = ok.size();
    std::cerr << "generated [" << nk << "/" << n << "]" << std::endl;
    alea.shuffle( *ok, ok.size() );
    
    checkPerf(s8,x8,ok);
    checkPerf(s16,x16,ok);
    checkPerf(s32,x32,ok);
    checkPerf(s64,x64,ok);

    
    
}
Y_UTEST_DONE()

