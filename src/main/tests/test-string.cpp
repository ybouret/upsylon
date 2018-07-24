#include "support.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

#define __DISP(X) std::cerr << #X "='" << X << "'" << std::endl

Y_UTEST(string)
{
    //! test length
    std::cerr << std::endl;
    std::cerr << length_of( ""    ) << std::endl;
    std::cerr << length_of("Hello") << std::endl;

    // constructor
    std::cerr << std::endl;
    string s1; Y_CHECK(s1.size()==0);__DISP(s1);
    string s2(12,as_capacity);Y_CHECK(s2.size()==0);Y_CHECK(s2.capacity()>=12);
    string s3( "Hello" );  Y_CHECK(s3.size()==5); __DISP(s3);
    string s4 = "World";   Y_CHECK(s4.size()==5); __DISP(s4);
    string s5 = (char *)0; Y_CHECK(s5.size()==0);
    string s6 = 'a';       Y_CHECK(s6.size()==1); __DISP(s6);

    // addition
    std::cerr << std::endl;
    string s7 = "Initial";
    s7 += s3;
    s7 += "And";
    s7 += 'C';
    __DISP(s7);

    string s8  = "hello" + s4;      __DISP(s8);
    string s9  = s3      + "world"; __DISP(s9);
    string s10 = 'H'     + s4;      __DISP(s10);
    string s11 = s3      + 'H';     __DISP(s11);

    std::cerr << std::endl;
    for(size_t i=0;i<10;++i)
    {
        const string lhs = support::get<string>();
        const string rhs = support::get<string>();
        const int    cmp = string::compare(lhs,rhs);
        std::cerr << "compare(" << lhs << "," << rhs <<")=" << cmp << std::endl;
        Y_CHECK(string::compare(lhs,lhs)==0);
    }

    std::cerr << std::endl;
    const char   *l  = "a";
    const char   *r  = "b";
    const char    L  = 'a';
    const char    R  = 'b';
    const string lhs = l;
    const string rhs = r;
    Y_CHECK(lhs<rhs);  Y_CHECK(lhs<r);  Y_CHECK(lhs<R);
    Y_CHECK(lhs<=rhs); Y_CHECK(lhs<=r); Y_CHECK(lhs<=R);
    Y_CHECK(lhs==lhs); Y_CHECK(lhs==l); Y_CHECK(lhs==L);
    Y_CHECK(lhs!=rhs); Y_CHECK(lhs!=r); Y_CHECK(lhs!=R);
    Y_CHECK(rhs>=lhs); Y_CHECK(rhs>=l); Y_CHECK(rhs>=L);
    Y_CHECK(rhs>lhs);  Y_CHECK(rhs>l);  Y_CHECK(rhs>L);

    std::cerr << "skip/trim" << std::endl;
    {
        string s;
        for(size_t i=0;i<80;++i)
        {
            s += alea.range<char>('!','~');
        }
        std::cerr << s << std::endl;
        while(s.size())
        {
            if( alea.choice() )
            {
                s.trim( alea.leq(10) );
            }
            else
            {
                s.skip( alea.leq(10) );
            }
            std::cerr << s << std::endl;
        }
    }

}
Y_UTEST_DONE()

