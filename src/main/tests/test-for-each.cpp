#include "y/iterate/for-each.hpp"
#include "y/utest/run.hpp"

#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"


using namespace upsylon;

namespace {

    template <typename T>
    static inline void display_const( const T &x )
    {
        std::cerr << ' ' << x;
    }

}


Y_UTEST(for_each)
{
    vector<double> vd;
    list<float>    lf;

    const vector<double> & cvd = vd;
    const list<float>    & clf = lf;

    for(size_t i=1+alea.leq(10);i>0;--i)
    {
        vd.push_back( alea.to<double>() );
        lf.push_back( alea.to<float> () );
    }

    for_each(vd,display_const<double>);  std::cerr << std::endl;
    for_each(lf,display_const<float>);   std::cerr << std::endl;
    for_each(cvd,display_const<double>); std::cerr << std::endl;
    for_each(clf,display_const<float>);  std::cerr << std::endl;

}
Y_UTEST_DONE()


