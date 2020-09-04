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


            const Rule & Scanner:: call(Plugin &plg)
            {
               return call(plg.label,plg.P, &plg, &Plugin::Initialize);
            }

        }

    }

}
