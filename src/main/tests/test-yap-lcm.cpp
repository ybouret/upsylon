#include "y/yap/lcm.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/list.hpp"
#include "y/string/convert.hpp"

using namespace upsylon;
using namespace yap;

Y_UTEST(yap_lcm)
{
    typedef number::utype utype;
    list<utype>           u;
    list<apn>             U;

    for(int i=1;i<argc;++i)
    {
        u << string_convert::to<size_t>(argv[i],"value");
        U << apn::parse(argv[i]);
    }
    std::cerr <<  "u=" << u << std::endl;
    std::cerr <<  "U=" << U << std::endl;

    std::cerr << "Using native  type:" << std::endl;
    apn l = lcm::of(u); std::cerr << "l=" << l << std::endl;

    std::cerr << "Using natural type:" << std::endl;
    apn L = lcm::of(U); std::cerr << "L=" << L << std::endl;

    std::cerr << "Checking divisibility..." << std::endl;
    for(list<apn>::iterator it=U.begin();it!=U.end();++it)
    {
        const apn & value = *it;
        Y_ASSERT( L.is_divisible_by(value) );
    }
    Y_CHECK( say("all good") );

    list<apq> Q;
    for(size_t i=1+alea.leq(10);i>0;--i)
    {
        const apq q(alea.range<unit_t>(-100,100),i);
        Q.push_back(q);
    }
    std::cerr << Q << std::endl;
    const integer fac = lcm::of_denominators(Q);
    std::cerr << "fac=" << fac << std::endl;
    for(list<apq>::iterator i=Q.begin();i!=Q.end();++i)
    {
        apq &q =(*i);
        q *= fac;
    }
    std::cerr << Q << std::endl;
}
Y_UTEST_DONE()

