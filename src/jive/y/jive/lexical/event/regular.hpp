
//! \file

#ifndef Y_JIVE_LEXICAL_EVENT_REGULAR_INCLUDED
#define Y_JIVE_LEXICAL_EVENT_REGULAR_INCLUDED 1

#include "y/jive/lexical/event.hpp"

namespace upsylon {

    namespace Jive {

        namespace Lexical {

            //------------------------------------------------------------------
            //
            //! base class for regular events
            /**
             regular events will produce or discard lexemes after a given action
             */
            //------------------------------------------------------------------
            class RegularEvent : public Event
            {
            public:
                //--------------------------------------------------------------
                //
                // types and definitions
                //
                //--------------------------------------------------------------
                //! type of regular event
                enum Type
                {
                    Forward, //!< will produce a lexeme
                    Discard  //!< will be discarded after action
                };

                //--------------------------------------------------------------
                //
                // C++
                //
                //--------------------------------------------------------------
                virtual ~RegularEvent() throw(); //!< destructor

                //--------------------------------------------------------------
                //
                // members
                //
                //--------------------------------------------------------------
                const Type type; //!< category of regular event

            protected:
                explicit RegularEvent(const Type, const Action &); //!< setup

            private:
                Y_DISABLE_ASSIGN(RegularEvent);
                virtual const void *self() const throw();
            };


        }

    }

}

#endif

