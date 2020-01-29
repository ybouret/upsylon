//! \file

#ifndef Y_LANG_LEXICAL_RULE_INCLUDED
#define Y_LANG_LEXICAL_RULE_INCLUDED 1

#include "y/lang/lexical/lexeme.hpp"
#include "y/lang/pattern.hpp"
#include "y/functor.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {
            //------------------------------------------------------------------
            //
            // Aliases
            //
            //------------------------------------------------------------------
            typedef arc_ptr<const Pattern>          Motif;  //!< shared pattern
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
                //! type of event
                enum Kind
                {
                    Regular, //!< for lexeme production  (Forward/Discard)
                    Control  //!< for translator control (Jump/Call/Back)
                };
                typedef arc_ptr<const Event> Pointer; //!< shared pointer

                //--------------------------------------------------------------
                //
                // C++
                //
                //--------------------------------------------------------------
                virtual ~Event() throw(); //!<destructor

                //--------------------------------------------------------------
                //
                // members
                //
                //--------------------------------------------------------------
                const   Kind   kind;   //!< category of event
                mutable Action action; //!< action to be taken

            protected:
                //! initialize
                explicit Event(const Kind k, const Action &a);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Event);
            };

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
            };

            typedef arc_ptr<RegularEvent> RegularCode; //!< shared pointer


            //------------------------------------------------------------------
            //
            //! forwarding event
            /**
             take action an produce lexeme
             */
            //------------------------------------------------------------------
            class OnForward : public RegularEvent
            {
            public:
                explicit OnForward(const Action &); //!< setup
                virtual ~OnForward() throw();       //!< cleanup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(OnForward);
            };

            //------------------------------------------------------------------
            //
            //! discarding event
            /**
             take action and discard lexeme
             */
            //------------------------------------------------------------------
            class OnDiscard : public RegularEvent
            {
            public:
                explicit OnDiscard(const Action &); //!< setup
                virtual ~OnDiscard() throw();       //!< cleanup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(OnDiscard);
            };

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

            //------------------------------------------------------------------
            //
            //! make a call
            /**
             change local scanner, recording caller
             */
            //------------------------------------------------------------------
            class OnCall : public ControlEvent
            {
            public:
                explicit OnCall(const string &, const Action &);  //!< setup
                virtual ~OnCall() throw();                        //!< cleanup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(OnCall);
            };

            //------------------------------------------------------------------
            //
            //! make a jump
            /**
             change local scanner without caller record
             */
            //------------------------------------------------------------------
            class OnJump : public ControlEvent
            {
            public:
                explicit OnJump(const string &, const Action &);  //!< setup
                virtual ~OnJump() throw();                        //!< cleanup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(OnJump);
            };

            //------------------------------------------------------------------
            //
            //! make a come back
            /**
             come back to a former caller
             */
            //------------------------------------------------------------------
            class OnBack : public ControlEvent
            {
            public:
                explicit OnBack(const Action &a);//!< setup
                virtual ~OnBack() throw();       //!< cleanup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(OnBack);
            };

            //------------------------------------------------------------------
            //
            //! lexical rule motif->action
            /**
             the motif cannot be weak
             */
            //------------------------------------------------------------------
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
