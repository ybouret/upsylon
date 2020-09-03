

#include "y/jive/lexical/event/control.hpp"

namespace upsylon {

    namespace Jive {

        namespace Lexical {

            ControlEvent:: ~ControlEvent() throw()
            {
            }


            ControlEvent:: ControlEvent(const Type    t,
                                        const Action &a,
                                        const Tag    &id) :
            Event(Control,a),
            type(t),
            label(id)
            {
                assert(Back!=type);
            }

            ControlEvent:: ControlEvent(const Action &a, const Tag &id) :
            Event(Control,a),
            type(Back),
            label(id)
            {
            }

            const void * ControlEvent:: self() const throw()
            {
                return static_cast<const ControlEvent *>(this);
            }

            const char * ControlEvent:: TypeText(const Type t) throw()
            {
                switch(t)
                {
                    case Jump: return "Jump";
                    case Call: return "Call";
                    case Back: return "Back";
                }
                return "";
            }

            const char * ControlEvent:: typeText() const throw()
            {
                return TypeText(type);
            }

        }

    }

}

