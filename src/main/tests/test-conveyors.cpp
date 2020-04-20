
#include "y/ios/conveyors.hpp"
#include "y/utest/sizeof.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ovstream.hpp"
#include "y/ios/imstream.hpp"
#include "y/ios/tools/vizible.hpp"

#include "support.hpp"

using namespace upsylon;

namespace {
    
}


Y_UTEST(conveyors)
{
    ios::conveyors & IO = ios::conveyors::instance();
    
    const ios::conveyor &dch = IO.query<double>(comms::homogeneous);
    const ios::conveyor &dcd = IO.query<double>(comms::distributed);
    const ios::conveyor &sch = IO.query<string>(comms::homogeneous);
    const ios::conveyor &scd = IO.query<string>(comms::distributed);

    std::cerr << "dch=" << dch << std::endl;
    std::cerr << "dcd=" << dcd << std::endl;
    std::cerr << "sch=" << sch << std::endl;
    std::cerr << "scd=" << scd << std::endl;

    const ios::conveyor &cplxf_h = IO.query<complex,float>(comms::homogeneous);
    const ios::conveyor &cplxd_d = IO.query<complex,double>(comms::distributed);
    std::cerr << "cplxf_h=" << cplxf_h << std::endl;
    std::cerr << "cplxd_d=" << cplxd_d << std::endl;
    
    std::cerr << IO.query<point3d,double>(comms::homogeneous) << std::endl;
    
    IO.root().graphViz("convey.dot");
    
    Y_UTEST_SIZEOF(ios::conveyor);
    Y_UTEST_SIZEOF(ios::primary_conveyor<char>);
    Y_UTEST_SIZEOF(ios::primary_conveyor<int>);
    Y_UTEST_SIZEOF(ios::network_conveyor<char>);
    Y_UTEST_SIZEOF(ios::network_conveyor<int>);
    Y_UTEST_SIZEOF(ios::derived_conveyor<string>);
    Y_UTEST_SIZEOF(ios::derived_conveyor<mpn>);
    typedef ios::tuple_conveyor<complex,float,ios::primary_conveyor> complex_float_conveyer;
    Y_UTEST_SIZEOF(complex_float_conveyer);
    typedef ios::tuple_conveyor<point2d,double,ios::network_conveyor> point3d_double_conveyer;
    Y_UTEST_SIZEOF(point3d_double_conveyer);

    IO.display();
    IO.sort();
    IO.display();
    
    IO.import();
    IO.display();

    
}
Y_UTEST_DONE()

