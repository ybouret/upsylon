#include "y/ios/scribe/db.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"

using namespace upsylon;

namespace
{

    template <typename T>
    inline void do_test()
    {
        ios::scribes &io = ios::scribes::instance();
        const T tmp = support::get<T>();
        
        std::cerr << tmp << " -> " << io.get<T>().write(&tmp) << std::endl;
    }

}

Y_UTEST(ios_scribe)
{
    ios::scribes &io = ios::scribes::instance();

    std::cerr << "#native: " << io.nat.size() << std::endl;

    for( ios::scribes::native_iterator it=io.nat.begin(); it!=io.nat.end();++it)
    {
        const ios::native_scribe &s = **it;
        std::cerr << s.info << " => '" << s.fmt << "'" << std::endl;
    }

    std::cerr << std::endl;
    std::cerr << "#common: " << io.all.size() << std::endl;
    for( ios::scribes::iterator it=io.all.begin(); it != io.all.end(); ++it)
    {
        const ios::scribe &s = **it;
        std::cerr << "-> " << s.info << std::endl;
    }

    do_test<int>();
    do_test<float>();
    do_test<double>();
    do_test<string>();
    do_test<apn>();
    do_test<apz>();
    do_test<apq>();

    do_test< complex<float> >();
    do_test< point2d<float> >();
    do_test< point3d<double> >();

}
Y_UTEST_DONE()

