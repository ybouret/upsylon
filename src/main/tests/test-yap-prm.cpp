
#include "y/yap/prime/iterator.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/icstream.hpp"
#include "y/sequence/vector.hpp"
#include "y/string/tokenizer.hpp"

using namespace upsylon;
using namespace yap;

#define ITER 128

namespace {



}
#include "y/string/convert.hpp"

Y_UTEST(yap_prm)
{
    if(argc>1)
    {
        vector<apn> plist;
        {
            const string   fn = argv[1];
            ios::icstream  fp(fn);
            vector<string> words(128,as_capacity);
            string line;
            while(fp.gets(line))
            {
                words.free();
                const size_t np = tokenizer<char>::split_with(words,line, " \t\r\n");
                for(size_t i=1;i<=np;++i)
                {
                    const apn n = natural::parse(words[i]);
                    plist << n;
                }
            }
        }
        std::cerr << "#parsed=" << plist.size() << std::endl;

        library         &apl = library::instance();
        prime_iterator p(apl);
        for(size_t i=1;i<=plist.size();++i,++p)
        {
            const natural &n = plist[i];
            std::cerr << '.';
            std::cerr.flush();
            Y_ASSERT(n==p);
            if( (0==(i%64) ) ) std::cerr << std::endl;
        }
        std::cerr << std::endl;
    }
}
Y_UTEST_DONE()
