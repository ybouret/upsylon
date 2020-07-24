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

        typedef list<string> Words;

        // initialize
        vector<Words> output(width,as_capacity);
        {
            const Words empty;
            output.adjust(width,empty);
        }

        suffix_store<char> db;

        // loop over word sizes
        size_t total = 0;
        for(size_t w=2;w<=width;++w)
        {
            std::cerr << "[" << w << "] : ";
            Words      &words = output[w];
            combination comb(width,w);
            string      draw(w,as_capacity,true);
            for(comb.boot();comb.good();comb.next())
            {
                // make a draw
                comb.apply(*draw,*input);
                string guess(w,as_capacity,true);

                // check permutations
                permutations<char> perm(*draw,w);
                for(perm.boot();perm.good();perm.next())
                {
                    perm.apply(*guess);
                    if( db.insert(*guess,w) )
                    {
                        words << guess;
                        ++total;
                    }
                }
            }
            std::cerr << words.size() << std::endl;
            std::cerr << words << std::endl;
        }
        std::cerr << "total=" << total << std::endl;



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
                std::cerr << "[" << w << "] : " ;
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



