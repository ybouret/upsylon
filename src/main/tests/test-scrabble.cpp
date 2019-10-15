#include "y/counting/comb.hpp"
#include "y/counting/perm.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"

#include "y/ordered/sorted-vector.hpp"
#include "y/ios/icstream.hpp"

using namespace upsylon;

typedef sorted_vector<string> words;

static inline bool WS(const int C) throw() { return C == ' ' || C == '\t' || C == '\r' || C == '\n'; }

Y_UTEST(scrabble)
{
    if(argc>1)
    {

        ordered_unique<words> db;

        const string original = argv[1];
        const size_t n        = original.size();
        size_t       total    = 0;
        for(size_t k=1;k<=n;++k)
        {
            size_t sub_total = 0;
            combination C(n,k);
            for( C.start(); C.valid(); C.next() )
            {
                string choice;
                for(size_t i=1;i<=C.size();++i) choice << original[ C[i]-1 ];
                permutation P( C.size() );
                for( P.start(); P.valid(); P.next() )
                {
                    string anagram;
                    for(size_t i=0;i<P.size();++i) anagram << choice[ P(i) ];
                    if( db.insert(anagram) )
                    {
                        ++sub_total;
                    }
                }
            }
            total += sub_total;
            Y_ASSERT(db.size()==total);
        }

        {
            size_t count = 0;
            for(size_t i=1;i<=total;++i)
            {
                std::cerr << db[i];
                if(++count>=8) { count = 0; std::cerr << std::endl;} else { std::cerr << ' '; }
            }
            std::cerr << std::endl;
        }

        if(argc>2)
        {
            const string fn = argv[2];
            std::cerr << "loading " << fn << "[";
            ios::icstream fp(argv[2]);
            ordered_single<words> dict(1024*1024,as_capacity);
            {
                string line;
                size_t count = 0;
                while( fp.gets(line) )
                {
                    line.clean(WS);
                    dict.insert(line);
                    if(++count>=1000)
                    {
                        count=0;
                        std::cerr << ".";
                    }
                }
            } std::cerr << "]" << std::endl;
            std::cerr << "Testing against |dict|=" << dict.size() << std::endl;
            for(size_t i=1;i<=total;++i)
            {
                const string &word = db[i];
                if( dict.search(word) )
                {
                    std::cerr << "\t" << word << std::endl;
                }
            }
        }
    }
}
Y_UTEST_DONE()



