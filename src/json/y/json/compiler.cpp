
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

        void  Compiler:: getValue(Value &value, const Jive::XNode *tree)
        {
            analyzer.reset();
            analyzer.walk( tree );
            if( analyzer.vstack.size() != 1)
            {
                throw exception("invalid JSON tree after compilation");
            }
            value.swapWith(analyzer.vstack.front());
            analyzer.reset();
        }
        
        
    }

}

