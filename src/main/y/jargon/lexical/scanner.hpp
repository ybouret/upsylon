

//! \file

#ifndef Y_JARGON_LEXICAL_SCANNER_INCLUDED
#define Y_JARGON_LEXICAL_SCANNER_INCLUDED 1

#include "y/jargon/lexical/rule.hpp"
#include "y/jargon/lexical/event/regular-family.hpp"
#include "y/jargon/lexical/event/control-family.hpp"

#include "y/jargon/lexical/unit.hpp"
#include "y/jargon/pattern/regexp.hpp"
#include "y/ptr/intr.hpp"
namespace upsylon {
    
    namespace Jargon {
        
        namespace Lexical {
            
            //! directive for Lexer during probe
            typedef const ControlEvent *Directive;
            
            //! trace calls
#define Y_JSCANNER(CODE) do { if(Scanner::Verbose) { CODE; } } while(false)
            
            //------------------------------------------------------------------
            //
            //! simple lexical scanner
            //
            //------------------------------------------------------------------
            class Scanner : public CountedObject, public inode<Scanner>
            {
            public:
                static  bool                     Verbose; //!< global lexical verbosity
                typedef intr_ptr<string,Scanner> Handle;  //!< for database
                const Tag label; //!< identifier
                
        
                
                explicit Scanner(const string &); //!< setup
                explicit Scanner(const Tag    &); //!< setup
                virtual ~Scanner() throw();       //!< cleanu[
                
                const string &key() const throw(); //!< for intr_ptr/set
                void          add(Rule *rule);     //!< add a rule, check no multiple
                
              
                void doNothing(const Token &) const throw(); //!< ...
                void doNewLine(const Token &) throw();       //!< send newLine to current source
                
                
                //! build a forwarding regular evne
                template <
                typename LABEL,
                typename REGEXP,
                typename OBJECT_POINTER,
                typename METHOD_POINTER
                >
                void forward(const LABEL   &anyLabel,
                             const REGEXP  &anyRegExp,
                             OBJECT_POINTER hObject,
                             METHOD_POINTER hMethod)
                {
                    regular<LABEL,REGEXP,OBJECT_POINTER,METHOD_POINTER,OnForward>(anyLabel,anyRegExp,hObject,hMethod);
                }
                
                
                //! build a discarding regular event
                template <
                typename LABEL,
                typename REGEXP,
                typename OBJECT_POINTER,
                typename METHOD_POINTER
                >
                void discard(const LABEL   &anyLabel,
                             const REGEXP  &anyRegExp,
                             OBJECT_POINTER hObject,
                             METHOD_POINTER hMethod)
                {
                    regular<LABEL,REGEXP,OBJECT_POINTER,METHOD_POINTER,OnDiscard>(anyLabel,anyRegExp,hObject,hMethod);
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
                    leap<LABEL,REGEXP,OBJECT_POINTER,METHOD_POINTER,OnCall>(target,regexp,hObject,hMethod,callPrefix);
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
                    leap<LABEL,REGEXP,OBJECT_POINTER,METHOD_POINTER,OnJump>(target,regexp,hObject,hMethod,jumpPrefix);
                }
                
                //! build a back
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
                    const string        backLabel = backPrefix + *label;// + '@' + rx;
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
                 -- if Directive==NULL, EOF
                 -- otherwise follow directive!
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
                
            private:
                template <
                typename LABEL,
                typename REGEXP,
                typename OBJECT_POINTER,
                typename METHOD_POINTER,
                typename REGULAR>
                void regular(const LABEL         &anyLabel,
                             const REGEXP        &anyRegExp,
                             const OBJECT_POINTER hObject,
                             const METHOD_POINTER hMethod)
                {
                    assert(hObject);
                    assert(hMethod);
                    const Tag            ruleLabel = new string(anyLabel);
                    const Motif          ruleMotif = RegularExpression::Compile(anyRegExp,dict);
                    const Action         ruleAction(hObject,hMethod);
                    const Event::Handle  ruleEvent  = new REGULAR(ruleAction);
                    add( new Rule(ruleLabel,ruleMotif,ruleEvent) );
                }
                
                template <
                typename LABEL,
                typename REGEXP,
                typename OBJECT_POINTER,
                typename METHOD_POINTER,
                typename LEAP
                >
                void leap(const LABEL   &target,
                          const REGEXP  &regexp,
                          OBJECT_POINTER hObject,
                          METHOD_POINTER hMethod,
                          const char     prefix[])
                {
                    const string         theTarget( target );
                    const string         theLabel  = prefix + theTarget;
                    const Tag            ruleLabel = new string(theLabel);
                    const Motif          ruleMotif = RegularExpression::Compile(regexp,dict);
                    const Action         ruleAction(hObject,hMethod);
                    const Event::Handle  ruleEvent = new LEAP(ruleAction,theTarget);
                    add( new Rule(ruleLabel,ruleMotif,ruleEvent) );
                }
            };
            
        }
        
    }
    
}

#endif

