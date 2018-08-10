#include "y/container/tuple.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"

using namespace upsylon;

namespace
{
    Y_PAIR_DECL(STANDARD,dummy2,int,v1,unsigned,v2);
    Y_PAIR_END();
    
    Y_TRIPLE_DECL(STANDARD,dummy3,long int,v1,unsigned,v2,size_t,v3);
    Y_TRIPLE_END();
    
    Y_QUAD_DECL(STANDARD,dummy4,int16_t,v1,int32_t,v2,int64_t,v3,char,v4);
    Y_QUAD_END();
    
    Y_PENTUPLE_DECL(STANDARD,dummy5,char,v1,short,v2,int,v3,long,v4,string,v5);
    Y_PENTUPLE_END();
}

Y_UTEST(tuple)
{
    {
        dummy2 d2a(1,2);
        dummy2 d2b(1,3);
        dummy2 d2c = d2a;
        Y_CHECK(d2a!=d2b);
        Y_CHECK(!(d2a==d2b));
        std::cerr << "d2a=" << d2a << std::endl;
        std::cerr << "d2b=" << d2b << std::endl;
    }
    
    {
        dummy3 d3a(1,2,3);
        dummy3 d3b(1,3,3);
        dummy3 d3c = d3a;
        Y_CHECK(d3a!=d3b);
        Y_CHECK(!(d3a==d3b));
        std::cerr << "d3a=" << d3a << std::endl;
        std::cerr << "d3b=" << d3b << std::endl;
    }
    
    {
        dummy4 d4a(1,2,3,'a');
        dummy4 d4b(1,2,3,'b');
        dummy4 d4c = d4a;
        Y_CHECK(d4a!=d4b);
        Y_CHECK(!(d4a==d4b));
        std::cerr << "d4a=" << d4a << std::endl;
        std::cerr << "d4b=" << d4b << std::endl;
    }
    
    {
        dummy5 d5a('a','b','c','d',"e");
        dummy5 d5b('a','b','c','d',"f");
        dummy5 d5c = d5a;
        Y_CHECK(d5a!=d5b);
        Y_CHECK(!(d5a==d5b));
        std::cerr << "d5a=" << d5a << std::endl;
        std::cerr << "d5b=" << d5b << std::endl;
    }
    
    
}
Y_UTEST_DONE()

