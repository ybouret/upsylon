
#include "y/jive/lexical/plugin/strings.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Lexical
        {

            String:: ~String() throw()
            {
            }

            void String:: setup()
            {
                back(delimiter,this,&String::OnQuit);
                discard("char", "[:core:]", this, &String::OnChar);


            }

            void String:: OnInit(const Token &t)
            {
                s.release();
                s << t;
            };

            void String:: OnQuit(const Token &t)
            {
                s << t;
                Unit *unit = new Unit(*(s.head),label);
                unit->swap_with(s);
                Q.push(unit);
            }

            void String:: OnChar(const Token &t)
            {
                s << t;
            }


        }

    }
}

