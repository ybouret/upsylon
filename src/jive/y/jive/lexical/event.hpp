

//! \file

#ifndef Y_JIVE_LEXICAL_EVENT_INCLUDED
#define Y_JIVE_LEXICAL_EVENT_INCLUDED 1

#include "y/jive/token.hpp"
#include "y/functor.hpp"

namespace upsylon {

    namespace Jive {

        namespace Lexical {

            typedef functor<void,TL1(const Token&)> Action; //!< executed on pattern recognition

            //--------------------------------------------------------------
            //
            //! base class for a scanner event = type+action
            /**
             an event is a wrapper for an action to take upon a pattern
             */
            //------------------------------------------------------------------
            class Event : public CountedObject
            {
            public:

                //--------------------------------------------------------------
                //
                // types and definitions
                //
                //--------------------------------------------------------------
                typedef arc_ptr<const Event> Handle; //!< alias

                //! handled kind of events
                enum  Kind
                {
                    Regular, //!< [Forward|Discard]
                    Control  //!< [Jump|Call|Back]
                };

                //--------------------------------------------------------------
                //
                // C++
                //
                //--------------------------------------------------------------
                virtual ~Event() throw();
                virtual  const void *self() const throw() = 0;

                //--------------------------------------------------------------
                //
                // members
                //
                //--------------------------------------------------------------
                const Kind   kind;    //!< kind of event
                const Action action;  //!< action to take upon recognition

            protected:
                //! setup
                explicit Event(const Kind, const Action &);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Event);
            };

        }

    }

}

#endif

