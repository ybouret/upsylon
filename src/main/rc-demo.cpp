#include "y/program.hpp"
#include "y/ios/rc/io.hpp"

using namespace upsylon;

Y_PROGRAM_START()
{
    std::cerr << std::endl;
    std::cerr << "<" << argv[0] << ">" << std::endl;
    const ios::rc::loader rc( argv[0], true );
    std::cerr << std::endl;
    std::cerr << "rc=" << rc.db() << std::endl;
    std::cerr << std::endl;
}
Y_PROGRAM_END()
