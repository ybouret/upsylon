#include "y/hashing/mph.hpp"
#include "y/associative/native-key.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"
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
        H.get_root().graphViz("words-mph.dot");
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
                    H(line,indx);
                    strings << line;
                }
            }
            if(H.size()<=30)
            {
                H.get_root().graphViz("mph.dot");
            }
            for(size_t i=strings.size();i>0;--i)
            {
                Y_ASSERT(i==H(strings[i]));
            }

            {
                ios::ocstream fp( ios::cstderr );
                H.emit(fp);
            }
        }

    }

    {
        hashing::minimal_perfect H;

        enum localInfo
        {
            first,
            second,
            third = 0x1234
        };

        native_key<uint32_t> k = unsigned(first);
        H(k,0);
        k.print_to(std::cerr) << std::endl;

        k=unsigned(second);
        H(k,2);
        k.print_to(std::cerr) << std::endl;

        k=unsigned(third);
        H(k,3);
        k.print_to(std::cerr) << std::endl;


        H.get_root().graphViz("mph-nat.dot");


    }

}
Y_UTEST_DONE()

