#include "y/mpl/rsa/keys.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"
#include "y/string/tokenizer.hpp"
#include "y/sequence/vector.hpp"
#include "y/codec/base64.hpp"
#include "y/string/convert.hpp"

using namespace upsylon;

static inline bool is_sep( const int C ) throw() { return C == ' ' || C == '\t'; }

Y_UTEST(rsa)
{
    if(argc>1)
    {
        const string rsa_pub = argv[1];
        string       data;
        {
            string        line;
            ios::icstream fp(rsa_pub);
            if( !fp.gets(line) )
            {
                throw exception("Empty %s", *rsa_pub);
            }
            vector<string> words(8,as_capacity);
            if(3!=tokenizer<char>::split(words,line,is_sep))
            {
                throw exception("Bad #words");
            }
            std::cerr << "words=" << words << std::endl;
            if("ssh-rsa"!=words[1])
            {
                throw exception("Bad first word '%s", *words[1] );
            }
            ios::base64::decoder dec;
            data = dec.to_string(words[2]);
        }
        std::cerr << "data=" << string_convert::to_printable(data) << std::endl;
    }
}
Y_UTEST_DONE()




