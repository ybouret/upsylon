

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
                static const char *KindText(const Kind) throw();
                
                //--------------------------------------------------------------
                //
                // C++
                //
                //--------------------------------------------------------------
                virtual ~Event() throw();  //!< cleanup

                //--------------------------------------------------------------
                //
                // methods
                //
                //--------------------------------------------------------------
                virtual  const void *self()     const throw() = 0; //!< derive class address
                virtual  const char *typeText() const throw() = 0; //!< derived type
                const  char         *kindText() const throw();     //!< KindText(kind)


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

