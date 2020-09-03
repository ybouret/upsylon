#include "y/jive/lexical/unit.hpp"


namespace upsylon
{
    namespace Jive
    {
        namespace Lexical
        {

            Unit:: ~Unit() throw()
            {
            }

            Unit:: Unit(const Context &c,
                        const Tag     &t) throw() :
            Token(), Context(c), inode<Unit>(), label(t)
            {
            }


        }

    }
}
