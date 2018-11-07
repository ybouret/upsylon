
#include "y/seem/compiler.hpp"

namespace upsylon
{
    namespace SEEM
    {

        Compiler:: ~Compiler() throw()
        {
        }


        namespace
        {
#include "seem.inc"
        }
        
        Compiler:: Compiler(const bool verbose ) :
        Lang::DynamoX(Y_DYNAMOX(SEEM), verbose )
        {
        }


    }

}


