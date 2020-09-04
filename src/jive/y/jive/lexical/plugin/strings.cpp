
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
                discard("char", "[:core:]", this, &String::OnCore);
                {
                    const char rx[4] = { '\\', '\\', delimiter,0};
                    discard("dlm",rx,this, &String::OnDelim);
                }
                
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

            void String:: OnCore(const Token &t)
            {
                s << t;
            }

            void String:: OnDelim(const Token &t)
            {
                assert(t.size==2);
                Char *ch = Char::Copycat(*t.tail);
                s << ch;
            }


        }

    }
}

