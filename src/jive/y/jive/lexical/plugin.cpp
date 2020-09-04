#include "y/jive/lexical/plugin.hpp"



namespace upsylon
{
    namespace Jive
    {
        namespace Lexical
        {

            Plugin:: ~Plugin() throw()
            {

            }

            void Plugin:: Initialize(const Token &_)
            {
                OnInit(_);
            }

        }

    }

}
