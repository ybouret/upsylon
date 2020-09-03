

#include "y/jive/lexical/event.hpp"

namespace upsylon {

    namespace Jive {

        namespace Lexical {

            
            Event:: ~Event() throw()
            {
            }

            Event:: Event(const Kind    k,
                          const Action &a) :
            kind(k),
            action(a)
            {
            }

            const  char * Event:: kindText() const throw()
            {
                return KindText(kind);
            }

            const char * Event::KindText(const Kind k) throw()
            {
                switch (k)
                {
                    case Regular: return "Regular";
                    case Control: return "Control";
                }
                return "";
            }
        }

    }
}
