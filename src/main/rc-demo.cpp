#include "y/program.hpp"
#include "y/ios/rc/io.hpp"

using namespace upsylon;

Y_PROGRAM_START()
{
    const ios::rc::loader rc( argv[0], true );

}
Y_PROGRAM_END()
