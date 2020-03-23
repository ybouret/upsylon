
//! \file

#ifndef Y_JARGON_LEXICAL_EVENT_INCLUDED
#define Y_JARGON_LEXICAL_EVENT_INCLUDED 1

#include "y/jargon/token.hpp"
#include "y/jargon/pattern.hpp"
#include "y/functor.hpp"

namespace upsylon {
    
    namespace Jargon {
        
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
                typedef arc_ptr<const Event> Handle;
                //--------------------------------------------------------------
                //
                // types and definitions
                //
                //--------------------------------------------------------------
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

                
                //--------------------------------------------------------------
                //
                // members
                //
                //--------------------------------------------------------------
                const Kind   kind;    //!< kind of event
                const Action action;  //!< action to take upon recognition
                const void  *self;    //!< derived type address
                
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

