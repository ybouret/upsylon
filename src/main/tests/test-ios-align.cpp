#include "y/ios/align.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;

Y_UTEST(ios_align)
{
    ios::ocstream fp( ios::cstderr );
    
    ios::align a("hello",3,ios::align::center,12);
    std::cerr <<'|' <<  a << '|' << '\n';
    fp <<'|' <<  a << '|' << '\n';

    std::cerr << '|' << ios::align("hello",ios::align::left,10) << '|' << std::endl;
    std::cerr << '|' << ios::align("world",ios::align::right,10) << '|' << std::endl;
    std::cerr << '|' << ios::align('Y',ios::align::center,10) << '|' << std::endl;
    const string s = "sample";
    std::cerr << '|' << ios::align(s,ios::align::center,10) << '|' << std::endl;


}
Y_UTEST_DONE()

