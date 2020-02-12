#include "y/information/filter/huffman.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/string.hpp"

using namespace upsylon;
using namespace information;

Y_UTEST(huffman)
{

    std::cerr << "Alphabet::Chars = " << Alphabet::Chars << std::endl;
    std::cerr << "Alphabet::NYT   = " << Alphabet::NYT   << std::endl;
    std::cerr << "Alphabet::EOS   = " << Alphabet::EOS   << std::endl;
    std::cerr << "Alphabet::Codes = " << Alphabet::Codes << std::endl;

    Y_UTEST_SIZEOF(Alphabet::Node);

}
Y_UTEST_DONE()

