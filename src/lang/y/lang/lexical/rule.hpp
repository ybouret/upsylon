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
            //! Pattern Shared Pointer
            typedef arc_ptr<const Pattern>          Motif;
            typedef functor<void,TL1(const Token&)> Action;

            class Event : public CountedObject
            {
            public:
                enum Type
                {
                    Regular,
                    Control
                };
                typedef arc_ptr<Event> Pointer;

                const Type   type;
                const Action action;
                inline virtual ~Event() throw() {}

            protected:
                inline Event(const Type t, const Action &a) : type(t), action(a) {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Event);
            };


            class RegularEvent : public Event
            {
            public:
                enum Type
                {
                    Forward,
                    Discard
                };
                const Type type;
                inline virtual ~RegularEvent() throw() {}

            protected:
                inline explicit RegularEvent(const Type t, const Action &a) : Event(Regular,a), type(t) {}

            private:
                Y_DISABLE_ASSIGN(RegularEvent);
            };

            typedef arc_ptr<RegularEvent> RegularCode;

            class OnForward : public RegularEvent
            {
            public:
                inline explicit OnForward(const Action &a) : RegularEvent(Forward,a) {}
                inline virtual ~OnForward() throw() {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(OnForward);
            };

            class OnDiscard : public RegularEvent
            {
            public:
                inline explicit OnDiscard(const Action &a) : RegularEvent(Discard,a) {}
                inline virtual ~OnDiscard() throw() {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(OnDiscard);
            };


            class ControlEvent : public Event
            {
            public:
                enum Type
                {
                    Call,
                    Back,
                    Jump
                };
                const Type   type;
                const string label;

            protected:
                ControlEvent(const Type    t,
                             const Action &a,
                             const string &l) :
                Event(Control,a), type(t), label(l) {}

                ControlEvent(const Action &a) : Event(Control,a), type(Back), label() {}


            private:
                Y_DISABLE_COPY_AND_ASSIGN(ControlEvent);
            };

            typedef arc_ptr<ControlEvent> ControlCode;

            class OnCall : public ControlEvent
            {
            public:
                inline explicit OnCall(const string &l, const Action &a) : ControlEvent(Call,a,l) {}
                inline virtual ~OnCall() throw() {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(OnCall);
            };

            class OnJump : public ControlEvent
            {
            public:
                inline explicit OnJump(const string &l, const Action &a) : ControlEvent(Jump,a,l) {}
                inline virtual ~OnJump() throw() {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(OnJump);
            };

            class OnBack : public ControlEvent
            {
            public:
                inline explicit OnBack(const Action &a) : ControlEvent(a) {}
                inline virtual ~OnBack() throw() {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(OnBack);
            };



            //! lexical rule motif->action
            class Rule : public Object
            {
            public:
                typedef core::list_of_cpp<Rule> List;

                Rule                 *next;   //!< for List
                Rule                 *prev;   //!< for List
                const Origin          label;  //!< identifier
                const Motif           motif;  //!< matching pattern
            private:
                const void           *handle;
            public:
                const Event::Pointer  event;

                virtual ~Rule() throw(); //!< destructor

                explicit Rule(const Origin      &ruleLabel,
                              const Motif       &ruleMotif,
                              const RegularCode &ruleEvent);

                explicit Rule(const Origin      &ruleLabel,
                              const Motif       &ruleMotif,
                              const ControlCode &ruleEvent);
            private:
                Y_DISABLE_ASSIGN(Rule);
            };

        }
    }
}

#endif
