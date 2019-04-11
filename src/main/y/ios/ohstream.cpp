#include "y/ios/ohstream.hpp"

namespace upsylon
{
    namespace ios
    {

        ohstream:: ~ohstream() throw()
        {
        }

        ohstream:: ohstream( hashing::function &H ) :
        hf(H)
        {
            //hf.set();
        }

        void ohstream:: write(char C)
        {
            hf(&C,1);
        }

        void ohstream:: flush()
        {
            
        }

    }
}
