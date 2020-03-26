

//! \file

#ifndef Y_JARGON_LEXICAL_SCANNER_INCLUDED
#define Y_JARGON_LEXICAL_SCANNER_INCLUDED 1

#include "y/jargon/lexical/rule.hpp"
#include "y/jargon/lexical/event/regular-family.hpp"
#include "y/jargon/lexical/event/control-family.hpp"

#include "y/jargon/lexical/unit.hpp"
#include "y/jargon/pattern/regexp.hpp"
#include "y/jargon/tags.hpp"
#include "y/ptr/intr.hpp"
namespace upsylon {
    
    namespace Jargon {
        
        namespace Lexical {
            
            class Plugin;
            
            //! directive for Lexer during probe
            /**
             a directive is emitted when a control event (jump/call/back) is
             triggered
             */
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
                //--------------------------------------------------------------
                //
                // types and definitions
                //
                //------------------------------------------------------------------
                static  bool                     Verbose; //!< global lexical verbosity
                typedef intr_ptr<string,Scanner> Handle;  //!< for database
                
                //--------------------------------------------------------------
                //
                // C++
                //
                //------------------------------------------------------------------
                explicit Scanner(const string &); //!< setup
                explicit Scanner(const Tag    &); //!< setup
                virtual ~Scanner() throw();       //!< cleanup
                
                //--------------------------------------------------------------
                //
                // generic method
                //
                //------------------------------------------------------------------
                const string &key() const throw();                   //!< for intr_ptr/set
                void          add(Rule *rule);                      //!< add a rule, check no multiple
                void          nothing(const Token &) const throw(); //!< ...
                void          newLine(const Token &) throw();       //!< send newLine to current source
                bool          isPlugin() const throw();             //!< check if plug in
                
                //------------------------------------------------------------------
                //
                //
                // forward/discard
                //
                //
                //------------------------------------------------------------------

                
                //! build a forwarding regular event
                /**
                 - action is taken
                 - unit is emitted
                 */
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
                /**
                 - action is taken
                 - unit is discarded and scanner goes on
                 */
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
                    forward(label,regexp,this,&Scanner::nothing);
                }
                
                //! default drop
                template <typename LABEL, typename REGEXP>
                void drop(const LABEL  &label, const REGEXP &regexp)
                {
                    discard(label,regexp,this,&Scanner::nothing);
                }
                
                //! default endl
                template <typename LABEL, typename REGEXP>
                void endl(const LABEL &label, const REGEXP &regexp)
                {
                    discard(label,regexp,this,&Scanner::newLine);
                }
                
                //------------------------------------------------------------------
                //
                //
                // controlling events
                //
                //
                //------------------------------------------------------------------

                
                //! build a call event
                /**
                 - action is taken
                 - lexer shall call the 'target' scanner
                 */
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
                    leap<LABEL,REGEXP,OBJECT_POINTER,METHOD_POINTER,OnCall>(target,regexp,hObject,hMethod);
                }
                
                //! build a jump event
                /**
                 - action is taken
                 - lexer shall jump to the 'target' scanner
                 */
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
                    leap<LABEL,REGEXP,OBJECT_POINTER,METHOD_POINTER,OnJump>(target,regexp,hObject,hMethod);
                }
                
                //! build a back event
                /**
                 - action is taken
                 - lexer shall go back to previous scanner
                 */
                template <
                typename REGEXP,
                typename OBJECT_POINTER,
                typename METHOD_POINTER
                >
                void back(const REGEXP  &regexp,
                          OBJECT_POINTER hObject,
                          METHOD_POINTER hMethod)
                {
                    const Motif         ruleMotif = RegularExpression::Compile(regexp,dict_);
                    const Action        ruleAction(hObject,hMethod);
                    const Event::Handle ruleEvent = new OnBack(ruleAction,label);
                    add( new Rule(label,ruleMotif,ruleEvent) );
                }
                
                
                
                //! the flex-like probing function
                /**
                 - if != NULL: a unit is produced
                 - if NULL   :
                 -- if Directive==NULL, EOF
                 -- otherwise follow directive!
                 */
                Lexical::Unit *probe(Source &, Directive &);
                
                const Tag label; //!< identifier
                
                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Scanner);
                Rules   rules;  // current rules
                Source *chars;  // current source
                friend class Analyzer;
                
                
                
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
                    const Tag            ruleLabel = Tags::Make(anyLabel);
                    const Motif          ruleMotif = RegularExpression::Compile(anyRegExp,dict_);
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
                          METHOD_POINTER hMethod)
                {
                    const Tag            ruleLabel = Tags::Make(target);
                    const Motif          ruleMotif = RegularExpression::Compile(regexp,dict_);
                    const Action         ruleAction(hObject,hMethod);
                    const Event::Handle  ruleEvent = new LEAP(ruleAction,ruleLabel);
                    add( new Rule(ruleLabel,ruleMotif,ruleEvent) );
                }
                
            protected:
                const Dictionary *dict_; //!< shared dictionary, default is NULL
                Plugin           *plug_; //!< if is a plugin
            };
            
        }
        
    }
    
}

#endif

