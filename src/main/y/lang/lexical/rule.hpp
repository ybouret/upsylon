//! \file

#ifndef Y_LANG_LEXICAL_RULE_INCLUDED
#define Y_LANG_LEXICAL_RULE_INCLUDED 1

#include "y/lang/lexical/unit.hpp"
#include "y/lang/pattern.hpp"
#include "y/functor.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {
            typedef arc_ptr<const Pattern>          Motif;  //!< shared pattern
            typedef functor<void,TL1(const Token&)> Action; //!< executed on pattern recognition

            //! base class for an scanner event
            class Event : public CountedObject
            {
            public:
                //! type of event
                enum Type
                {
                    Regular, //!< for lexeme production  (Forward/Discard)
                    Control  //!< for translator control (Jump/Call/Back)
                };

                typedef arc_ptr<Event> Pointer; //!< shared pointer

                const   Type   type;   //!< category of event
                mutable Action action; //!< action to be taken
                
                //! destructor
                virtual ~Event() throw();

            protected:
                //! initialize
                explicit Event(const Type t, const Action &a);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Event);
            };

            //! base class for regular events
            class RegularEvent : public Event
            {
            public:
                //! type of regular event
                enum Type
                {
                    Forward, //!< will produce a lexeme
                    Discard  //!< will be discarded after action
                };
                const Type type; //!< category of regular event

                //! destructor
                virtual ~RegularEvent() throw();

            protected:
                //! initialize
                explicit RegularEvent(const Type t, const Action &a);

            private:
                Y_DISABLE_ASSIGN(RegularEvent);
            };

            typedef arc_ptr<RegularEvent> RegularCode; //!< shared pointer

            //! forwarding event
            class OnForward : public RegularEvent
            {
            public:
                //! initialize
                explicit OnForward(const Action &a);

                //! destructor
                virtual ~OnForward() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(OnForward);
            };

            //! discarding event
            class OnDiscard : public RegularEvent
            {
            public:
                //! initialize
                explicit OnDiscard(const Action &a);

                //! desctructor
                virtual ~OnDiscard() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(OnDiscard);
            };

            //! base class for control events
            class ControlEvent : public Event
            {
            public:
                //! category of control event
                enum Type
                {
                    Call, //!< call a sub scanner
                    Back, //!< back from a sub scanner
                    Jump  //!< jump to a new scanner
                };
                const Type   type;  //!< category of control event
                const string label; //!< sub scanner to call, empty for back

                virtual ~ControlEvent() throw();

            protected:
                //! initialize call/jump
                explicit  ControlEvent(const Type    t,
                                       const Action &a,
                                       const string &l);

                //! initialize for back
                explicit ControlEvent(const Action &a);


            private:
                Y_DISABLE_COPY_AND_ASSIGN(ControlEvent);
            };

            typedef arc_ptr<ControlEvent> ControlCode; //!< shared control event

            //! make a call
            class OnCall : public ControlEvent
            {
            public:
                //!initialize
                explicit OnCall(const string &l, const Action &a);

                //!destructor
                virtual ~OnCall() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(OnCall);
            };

            //! make a jump
            class OnJump : public ControlEvent
            {
            public:
                //!initialize
                explicit OnJump(const string &l, const Action &a);

                //!destructor
                virtual ~OnJump() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(OnJump);
            };

            //! make a come back
            class OnBack : public ControlEvent
            {
            public:
                //! initialize
                explicit OnBack(const Action &a);
                
                //! destructor
                virtual ~OnBack() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(OnBack);
            };
            
            //! lexical rule motif->action
            class Rule : public Object
            {
            public:
                typedef core::list_of_cpp<Rule> List; //!< for scanner rules

                Rule                 *next;   //!< for List
                Rule                 *prev;   //!< for List
                const Tag             label;  //!< identifier
                const Motif           motif;  //!< matching pattern
                const void           *handle; //!< regular/control event address
                const Event::Pointer  event;  //!< shared event

                virtual ~Rule() throw(); //!< destructor

                //! setup a rule based on a regular event
                explicit Rule(const Tag         &ruleLabel,
                              const Motif       &ruleMotif,
                              const RegularCode &ruleEvent);

                //! setup a rule based on a control event
                explicit Rule(const Tag         &ruleLabel,
                              const Motif       &ruleMotif,
                              const ControlCode &ruleEvent);
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Rule);
                void checkMotif() const;
            };

        }
    }
}

#endif
