//! \file

#ifndef Y_JIVE_LEXICAL_EVENT_CONTROL_INCLUDED
#define Y_JIVE_LEXICAL_EVENT_CONTROL_INCLUDED 1

#include "y/jive/lexical/event.hpp"

namespace upsylon {

    namespace Jive {

        namespace Lexical {


            //------------------------------------------------------------------
            //
            //! base class for control events
            /**
             a control event will change a translator's state
             */
            //------------------------------------------------------------------
            class ControlEvent : public Event
            {
            public:
                //--------------------------------------------------------------
                // types and definitions
                //--------------------------------------------------------------

                //! category of control event
                enum Type
                {
                    Call, //!< call a sub scanner
                    Back, //!< back from a sub scanner
                    Jump  //!< jump to a new scanner
                };
                static const char * TypeText(const Type) throw();

                //--------------------------------------------------------------
                // C++
                //--------------------------------------------------------------
                virtual ~ControlEvent() throw();
                virtual  const char *typeText() const throw();

                //--------------------------------------------------------------
                // members
                //--------------------------------------------------------------
                const Type      type;    //!< category of control event
                const Tag       label;   //!< name to jump/call to, info for back

            protected:
                //! initialize call/jump
                explicit  ControlEvent(const Type,const Action &,const Tag &);

                //! initialize back
                explicit  ControlEvent(const Action &a, const Tag &);


            private:
                Y_DISABLE_COPY_AND_ASSIGN(ControlEvent);
                virtual const void *self() const throw();
            };




        }

    }

}

#endif

