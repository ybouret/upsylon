

//! \file

#ifndef Y_JARGON_LEXICAL_SCANNER_INCLUDED
#define Y_JARGON_LEXICAL_SCANNER_INCLUDED 1

#include "y/jargon/lexical/rule.hpp"
#include "y/jargon/lexical/event/regular-family.hpp"
#include "y/jargon/lexical/event/control-family.hpp"

#include "y/jargon/lexical/unit.hpp"
#include "y/jargon/pattern/regexp.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        namespace Lexical {
            
            //! directive for Lexer during probe
            typedef const ControlEvent *Directive;
            
            //------------------------------------------------------------------
            //! simple lexical scanner
            //------------------------------------------------------------------
            class Scanner : public CountedObject
            {
            public:
                const string name; //!< identifier
                
                explicit Scanner( const string &id ); //!< setup
                virtual ~Scanner() throw();           //!< cleanu[
                
                void add(Rule *rule);  //!< add a rule, check no multiple
                
                bool building() const throw(); //!< check building state
                void finish() throw();         //!< finish, release extra memory
                void resume();                 //!< resume building
                
                void doNothing(const Token &) const throw(); //!< ...
                void doNewLine(const Token &) throw();       //!< send newLine to current source
                
                
                //! build a regular event
                template <
                typename LABEL,
                typename REGEXP,
                typename OBJECT_POINTER,
                typename METHOD_POINTER,
                typename REGULAR>
                void regular(const LABEL         &label,
                             const REGEXP        &regexp,
                             const OBJECT_POINTER hObject,
                             const METHOD_POINTER hMethod)
                {
                    assert(hObject);
                    assert(hMethod);
                    const Motif          ruleMotif = RegularExpression::Compile(regexp,dict);
                    const Tag            ruleLabel = new string(label);
                    const Action         ruleAction(hObject,hMethod);
                    const Event::Handle  ruleEvent  = new REGULAR(ruleAction);
                    add( new Rule(ruleLabel,ruleMotif,ruleEvent) );
                }
                
                //! build a forwarding regular evne
                template <
                typename LABEL,
                typename REGEXP,
                typename OBJECT_POINTER,
                typename METHOD_POINTER
                >
                void forward(const LABEL   &label,
                             const REGEXP  &regexp,
                             OBJECT_POINTER hObject,
                             METHOD_POINTER hMethod)
                {
                    regular<LABEL,REGEXP,OBJECT_POINTER,METHOD_POINTER,OnForward>(label,regexp,hObject,hMethod);
                }
                
                
                //! build a discarding regular event
                template <
                typename LABEL,
                typename REGEXP,
                typename OBJECT_POINTER,
                typename METHOD_POINTER
                >
                void discard(const LABEL   &label,
                             const REGEXP  &regexp,
                             OBJECT_POINTER hObject,
                             METHOD_POINTER hMethod)
                {
                    regular<LABEL,REGEXP,OBJECT_POINTER,METHOD_POINTER,OnDiscard>(label,regexp,hObject,hMethod);
                }
                
                //! default emit
                template <typename LABEL, typename REGEXP>
                void emit(const LABEL  &label, const REGEXP &regexp)
                {
                    forward(label, regexp, this, & Scanner:: doNothing );
                }
                
                //! default drop
                template <typename LABEL, typename REGEXP>
                void drop(const LABEL  &label, const REGEXP &regexp)
                {
                    discard(label,regexp,this,&Scanner::doNothing);
                }
                
                //! default endl
                template <typename LABEL, typename REGEXP>
                void endl(const LABEL &label, const REGEXP &regexp)
                {
                    discard(label,regexp,this,&Scanner::doNewLine);
                }
                
                
                //! build a call
                template <
                typename LABEL,
                typename REGEXP,
                typename OBJECT_POINTER,
                typename METHOD_POINTER
                >
                void call(const LABEL   &target,
                          const REGEXP  &regexp,
                          OBJECT_POINTER hObject,
                          METHOD_POINTER hMethod)
                {
                    const string         callTarget( target );
                    const string         callLabel = callPrefix + callTarget;
                    const Motif          ruleMotif = RegularExpression::Compile(regexp,dict);
                    const Tag            ruleLabel = new string(callLabel);
                    const Action         ruleAction(hObject,hMethod);
                    const Event::Handle  ruleEvent = new OnCall(ruleAction,callTarget);
                    add( new Rule(ruleLabel,ruleMotif,ruleEvent) );
                }
                
                //! build a jump
                template <
                typename LABEL,
                typename REGEXP,
                typename OBJECT_POINTER,
                typename METHOD_POINTER
                >
                void jump(const LABEL   &target,
                          const REGEXP  &regexp,
                          OBJECT_POINTER hObject,
                          METHOD_POINTER hMethod)
                {
                    const string         jumpTarget( target );
                    const string         jumpLabel = jumpPrefix + jumpTarget;
                    const Motif          ruleMotif = RegularExpression::Compile(regexp,dict);
                    const Tag            ruleLabel = new string(jumpLabel);
                    const Action         ruleAction(hObject,hMethod);
                    const Event::Handle  ruleEvent = new OnJump(ruleAction,jumpTarget);
                    add( new Rule(ruleLabel,ruleMotif,ruleEvent) );
                }
                
                //! build a nack
                template <
                typename REGEXP,
                typename OBJECT_POINTER,
                typename METHOD_POINTER
                >
                void back(const REGEXP  &regexp,
                          OBJECT_POINTER hObject,
                          METHOD_POINTER hMethod)
                {
                    const string        rx(regexp);
                    const string        backLabel = backPrefix + name + '@' + rx;
                    const Motif         ruleMotif = RegularExpression::Compile(rx,dict);
                    const Tag           ruleLabel = new string(backLabel);
                    const Action        ruleAction(hObject,hMethod);
                    const Event::Handle ruleEvent = new OnBack(ruleAction);
                    add( new Rule(ruleLabel,ruleMotif,ruleEvent) );
                }
                
                
                //! the flex-like probing function
                /**
                 - if != NULL: a unit is produced
                 - if NULL   :
                 */
                Lexical::Unit *probe(Source &, Directive &);
                
                static const char callPrefix[]; //!< used to build inline call label
                static const char jumpPrefix[]; //!< used to build inline jump label
                static const char backPrefix[]; //!< used to build inline call label
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Scanner);
                Rules   rules;  // current rules
                Source *chars;  // current source
                void   *rdict;  // rule dictionary
                
            public:
                const Dictionary *dict; //!< shared dictionary, default is NULL
            };
            
        }
        
    }
    
}

#endif

