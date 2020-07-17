#include "y/counting/comb.hpp"
#include "y/counting/perm.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"
#include "y/string/display.hpp"

#include "y/ordered/sorted-vector.hpp"
#include "y/associative/suffix/tree.hpp"
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
            for( C.boot(); C.good(); C.next() )
            {
                string choice;
                for(size_t i=0;i<C.size();++i) choice << original[ C(i) ];
                permutation P( C.size() );
                for( P.boot(); P.good(); P.next() )
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
                string_display::align(std::cerr <<'<', db[i], n) << '>';
                if(++count>=8) { count = 0; std::cerr << std::endl;} else { std::cerr << ' '; }
            }
            std::cerr << std::endl;
            std::cerr << "[" << db.size() << "]" << std::endl;
        }

        if(argc>2)
        {
            const string fn = argv[2];
            std::cerr << "loading " << fn << "[";
            ios::icstream fp(argv[2]);

            suffix_tree<bool> dict;
            {
                string line;
                size_t count = 0;
                while( fp.gets(line) )
                {
                    line.clean(WS);
                    (void) dict.insert_by(line,true);
                    if(++count>=1000)
                    {
                        count=0;
                        std::cerr << ".";
                    }
                }
            } std::cerr << "]" << std::endl;

            std::cerr << "Testing against |dict|=" << dict.entries() << std::endl;
            {
                size_t count = 0;
                for(size_t i=1;i<=total;++i)
                {
                    const string &word = db[i];
                    if(word.size()<=1) continue;
                    if( dict.has(word) )
                    {
                        string_display::align(std::cerr, word, n);
                        if(++count>=8) { count = 0; std::cerr << std::endl;} else { std::cerr << ' '; }
                    }
                    //std::cerr << "[" << db.size() << "]" << std::endl;
                }
                std::cerr << std::endl;
            }
        }
    }
}
Y_UTEST_DONE()



