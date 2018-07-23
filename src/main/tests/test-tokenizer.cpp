#include "y/string/tokenizer.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace
{
    static inline bool is_sep(const char C )
    {
        return C==' ' || C=='\t';
    }
}
Y_UTEST(tokenizer)
{
    string line = "   Hello ,    world!  ";
    tokenizer<char> tkn(line);
    while( tkn.next(is_sep) )
    {
        const string word = tkn.to_string();
        std::cerr << "token#" << tkn.count() << "='" << word << "'" << std::endl;
    }

}
Y_UTEST_DONE()

