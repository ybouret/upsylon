
#include "y/lang/lexical/rule.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {
            Event:: ~Event() throw() {}
            Event:: Event(const Kind k, const Action &a) : kind(k), action(a) {}

            //
            RegularEvent:: ~RegularEvent() throw() {}
            RegularEvent::  RegularEvent(const Type t, const Action &a) : Event(Regular,a), type(t) {}

            //
            OnForward::  OnForward(const Action &a) : RegularEvent(Forward,a) {}
            OnForward:: ~OnForward() throw()  {}

            //
            OnDiscard::  OnDiscard(const Action &a) : RegularEvent(Discard,a) {}
            OnDiscard:: ~OnDiscard() throw() {}

            //
            ControlEvent:: ~ControlEvent() throw() {}

            ControlEvent:: ControlEvent(const Type    t,
                                        const Action &a,
                                        const string &l) : Event(Control,a), type(t), label(l)
            { assert(Back!=type); }

            ControlEvent:: ControlEvent(const Action &a) : Event(Control,a), type(Back), label() {}

            //
            OnCall::  OnCall(const string &l, const Action &a) : ControlEvent(Call,a,l) {}
            OnCall:: ~OnCall() throw() {}

            //
            OnJump::  OnJump(const string &l, const Action &a) : ControlEvent(Jump,a,l) {}
            OnJump:: ~OnJump() throw() {}

            //
            OnBack::  OnBack(const Action &a) : ControlEvent(a) {}
            OnBack:: ~OnBack() throw() {}

        }
    }
}


namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {
            Rule:: ~Rule() throw()
            {
            }


            Rule:: Rule(const Tag         &ruleLabel,
                        const Motif       &ruleMotif,
                        const RegularCode &ruleEvent) :
            Object(),
            next(0),
            prev(0),
            label(ruleLabel),
            motif(ruleMotif),
            handle( & *ruleEvent ),
            event( (RegularEvent*)handle )
            {
                checkMotif();
            }

            Rule:: Rule(const Tag         &ruleLabel,
                        const Motif       &ruleMotif,
                        const ControlCode &ruleEvent) :
            next(0),
            prev(0),
            label(ruleLabel),
            motif(ruleMotif),
            handle( & *ruleEvent ),
            event( (ControlEvent*)handle )
            {
                checkMotif();
            }


            void Rule:: checkMotif() const
            {
                if(motif->weak())
                {
                    const char *rid = **label;
                    const char *tid = "Regular";
                    if(event->kind==Event::Control)
                    {
                        tid = "Control";
                    }
                    throw exception("Lexical::Rule <%s>/%s : Weak Pattern", rid,tid);
                }
            }
        }
    }
}
