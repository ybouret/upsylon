#include "support.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

#define __DISP(X) std::cerr << #X "='" << X << "'" << std::endl

Y_UTEST(string)
{
    //! test length
    std::cerr << length_of( ""    ) << std::endl;
    std::cerr << length_of("Hello") << std::endl;

    // constructor
    string s1; Y_CHECK(s1.size()==0);__DISP(s1);
    string s2(12,as_capacity);Y_CHECK(s2.size()==0);Y_CHECK(s2.capacity()>=12);
    string s3( "Hello" );  Y_CHECK(s3.size()==5); __DISP(s3);
    string s4 = "World";   Y_CHECK(s4.size()==5); __DISP(s4);
    string s5 = (char *)0; Y_CHECK(s5.size()==0);
    string s6 = 'a';       Y_CHECK(s6.size()==1); __DISP(s6);

    // addition
    string s7 = "Initial";
    s7 += s3;
    s7 += "And";
    s7 += 'C';
    __DISP(s7);

    string s8  = "hello" + s4;      __DISP(s8);
    string s9  = s3      + "world"; __DISP(s9);
    string s10 = 'H'     + s4;      __DISP(s10);
    string s11 = s3      + 'H';     __DISP(s11);

    for(size_t i=0;i<10;++i)
    {
        const string lhs = support::get<string>();
        const string rhs = support::get<string>();
        const int    cmp = string::compare(lhs,rhs);
        std::cerr << "compare(" << lhs << "," << rhs <<")=" << cmp << std::endl;
        Y_CHECK(string::compare(lhs,lhs)==0);
    }



}
Y_UTEST_DONE()

