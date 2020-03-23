

//! \file

#ifndef Y_JARGON_LEXICAL_SCANNER_INCLUDED
#define Y_JARGON_LEXICAL_SCANNER_INCLUDED 1

#include "y/jargon/lexical/rule.hpp"
#include "y/jargon/lexeme.hpp"
#include "y/jargon/pattern/regexp.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        namespace Lexical {
            
            class Scanner : public CountedObject
            {
            public:
                const string name;
                
                explicit Scanner( const string &id );
                virtual ~Scanner() throw();
                
                void add(Rule *rule);
                
                bool building() const throw();
                void finish() throw();
                void resume();
                
                void doNothing(const Token &) const throw();
                void doNewLine(const Token &) throw();
                
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
                
                template <typename LABEL, typename REGEXP>
                void emit(const LABEL  &label, const REGEXP &regexp)
                {
                    forward(label, regexp, this, & Scanner:: doNothing );
                }
                
                template <typename LABEL, typename REGEXP>
                void drop(const LABEL  &label, const REGEXP &regexp)
                {
                    discard(label,regexp,this,&Scanner::doNothing);
                }
                
                
                template <typename LABEL, typename REGEXP>
                void endl(const LABEL &label, const REGEXP &regexp)
                {
                    discard(label,regexp,this,&Scanner::doNewLine);
                }
                
                Lexical::Unit *probe(Source &);
                
                
                
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

