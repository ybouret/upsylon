#include "y/string.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

#define __DISP(X) std::cerr << #X "='" << X << "'" << std::endl

Y_UTEST(string)
{
    std::cerr << length_of( ""    ) << std::endl;
    std::cerr << length_of("Hello") << std::endl;

    string s1;
    Y_CHECK(s1.size()==0);
    __DISP(s1);
    string s2(12,as_capacity);Y_CHECK(s2.size()==0);
    string s3( "Hello" );
    string s4 = "World";
    string s5 = (char *)0;
    string s6 = 'a';
}
Y_UTEST_DONE()

