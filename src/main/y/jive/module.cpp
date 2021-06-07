
#include "y/jive/module.hpp"
#include "y/type/aliasing.hpp"

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
                Char *ch = Char::Acquire(*this,C);
                aliasing::incr(column);
                return ch;
            }
            else
            {
                return NULL;
            }
        }

        void Module:: newLine() throw()
        {
            aliasing::_(column) = 1;
            aliasing::incr(line);
        }

       



    }
}
