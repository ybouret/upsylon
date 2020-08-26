
#include "y/jive/module.hpp"

namespace upsylon {

    namespace Jive
    {
        Module:: ~Module() throw()
        {
        }

        Module:: Module(const Tag   &tag,
                        const Input &inp,
                        const Type   tid) throw() :
        Context(tag),
        input(inp),
        type(tid)
        {

        }

        Char *Module:: getChar()
        {
            char C = 0;
            if(input->query(C))
            {
                ++aliasing::_(column);
                return Char::Acquire(*this,C);
            }
            else
            {
                return NULL;
            }
        }

        void Module:: newLine() throw()
        {
            aliasing::_(column) = 1;
            ++aliasing::_(line);
        }

       



    }
}
