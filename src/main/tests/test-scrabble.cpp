#include "y/counting/comb.hpp"
#include "y/counting/permutations.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"
#include "y/string/display.hpp"


#include "y/ios/icstream.hpp"
#include "y/sequence/list.hpp"
#include "y/sequence/vector.hpp"
#include "y/associative/suffix/store.hpp"

using namespace upsylon;



Y_UTEST(scrabble)
{



    if(argc>1)
    {
        const string       input = argv[1];
        const size_t       width = input.size();
        string             reading(width,as_capacity,true);

        // initialize
        vector< list<string> > output(width,as_capacity);
        {
            const list<string> empty;
            for(size_t i=width;i>0;--i) output.push_back_(empty);
        }

        // loop
        {
            suffix_store<char> db;
            permutations<char> outer(*input,width);
            for( outer.boot(); outer.good(); outer.next() )
            {
                outer.apply(*reading);
                for(size_t w=1;w<=width;++w)
                {
                    combination comb(width,w);
                    string      word(w,as_capacity,true);
                    for(comb.boot();comb.good();comb.next())
                    {
                        comb.apply(*word,*reading);
                        if( db.insert(*word,w) )
                        {
                            output[w] << word;
                        }
                    }
                }
            }
        }

        size_t total = 0;
        for(size_t w=1;w<=width;++w)
        {
            list<string> &L = output[w];
            std::cerr << "[" << w << "] : " << L.size() << std::endl;
            std::cerr << L << std::endl;
            total += L.size();
        }
        std::cerr << "total: " << total << std::endl;


        if(argc>2)
        {
            const string fn = argv[2];
            std::cerr << "loading " << fn << "[";
            ios::icstream fp(argv[2]);
            
            suffix_store<char> dict;
            {
                string line;
                size_t count = 0;
                while( fp.gets(line) )
                {
                    line.clean_with(" \t\r\n");
                    (void) dict.insert(*line,line.size());
                    if(++count>=1000)
                    {
                        count=0;
                        std::cerr << ".";
                    }
                }
            } std::cerr << "]" << std::endl;

            std::cerr << "Testing against |dict|=" << dict.entries << " [#nodes=" << dict.nodes << "]" << std::endl;
            total=0;
            for(size_t w=1;w<=width;++w)
            {
                const list<string> &L = output[w];
                std::cerr << "[" << w << "] : " << L.size() << std::endl;
                for( list<string>::const_iterator i=L.begin();i!=L.end();++i)
                {
                    const string &word = *i; assert(word.size()==w);
                    if(dict.has(*word,w))
                    {
                        ++total;
                        std::cerr << " '" << word << "'";
                    }
                }
                std::cerr << std::endl;
            }
        }
    }


}
Y_UTEST_DONE()



