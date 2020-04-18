
#include "y/ios/conveyors.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ovstream.hpp"
#include "y/ios/imstream.hpp"
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

    
}
Y_UTEST_DONE()

