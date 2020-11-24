
#include "y/json/compiler.hpp"

namespace upsylon
{

    namespace JSON
    {
        Compiler:: Compiler() : parser(), analyzer( parser.name )
        {
        }

        Compiler:: ~Compiler() throw()
        {
        }

        Value & Compiler:: getValue()
        {
            if( analyzer.vstack.size() != 1)
            {
                throw exception("invalid JSON tree after compilation");
            }
            return analyzer.vstack.front();
        }
        
        
    }

}

