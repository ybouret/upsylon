#include "y/hashing/mph.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/ios/icstream.hpp"
#include "y/string.hpp"

using namespace upsylon;



Y_UTEST(mperf)
{

    const char *words[] =
    {
        "hello",
        "hell",
        "world",
        "word"
    };

    {
        hashing::minimal_perfect H( Y_MPH_WORDS(words) );
        H.graphViz("words-mph.dot");
    }


    {
        hashing::minimal_perfect H;

        if( argc>1 )
        {
            vector<string> strings;
            {
                ios::icstream fp(argv[1]);
                string        line;
                int           indx = 0;
                while( fp.gets(line) )
                {
                    ++indx;
                    H.add(line,indx);
                    strings << line;
                    //std::cerr << line << " -> " << indx << std::endl;
                }
            }
            if(H.entries()<=30)
            {
                H.graphViz("mph.dot");
            }
            for(size_t i=strings.size();i>0;--i)
            {
                Y_ASSERT(i==H(strings[i]));
            }
        }



    }


}
Y_UTEST_DONE()

