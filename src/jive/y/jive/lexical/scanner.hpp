//! \file

#ifndef Y_JIVE_LEXICAL_SCANNER_INCLUDED
#define Y_JIVE_LEXICAL_SCANNER_INCLUDED 1

#include "y/jive/lexical/rule.hpp"
#include "y/jive/lexical/events.hpp"
#include "y/jive/lexical/unit.hpp"
#include "y/jive/regexp.hpp"
#include "y/jive/scatter.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Lexical
        {

            class Plugin; //!< forward declaration

            //__________________________________________________________________
            //
            //! directive for Lexer during probe
            /**
             a directive is emitted when a control event (jump/call/back) is
             triggered
             */
            //__________________________________________________________________

            typedef const ControlEvent *Directive;

            //__________________________________________________________________
            //
            //
            //! detect and apply best rule to produce a token or a directive
            //
            //__________________________________________________________________
            class Scanner : public CountedObject
            {
            public:
                //--------------------------------------------------------------
                //
                // types and defintions
                //
                //--------------------------------------------------------------
                static  bool                     Verbose; //!< global lexical verbosity
                typedef prefix_storage<Rule*>    RulesDB; //!< rules database
                typedef arc_ptr<Scanner>         Pointer; //!< for lexer
                
                //! what happens when End of Stream if met during a call
                enum AtEOS
                {
                    RejectEOS, //!< will fail (and this is the default behavior!)
                    AcceptEOS  //!< ok, may stop
                };

                //! return text
                static const char *AtEOSText(const AtEOS) throw();

                //--------------------------------------------------------------
                //
                // C++
                //
                //--------------------------------------------------------------

                //! cleanup
                virtual ~Scanner() throw();

                //! setup using Tags
                template <typename ID> inline
                explicit Scanner(const ID &id, const AtEOS which=AcceptEOS) :
                label( Tags::Make(id) ),
                atEOS(which),
                rules(),
                hoard(),
                intro(),
                table(),
                origin(0),
                lmax(0),
                dict(0)
                {
                }


                //--------------------------------------------------------------
                //
                // methods
                //
                //--------------------------------------------------------------

                //! rule full registration
                /**
                 - r is added to internal rules
                 - the first chars of r->motifs are included in first chars db
                 - r is scattered into the table for all its first chars
                 */
                const Rule & add(Rule *r);


                const string & key()    const throw();  //!< get key for pointer
                const char   * getEOS() const throw();  //!< named atEOS

                //--------------------------------------------------------------
                //
                // members
                //
                //--------------------------------------------------------------
                const Tag    label; //!< label for this rule
                const AtEOS  atEOS; //!< what happens if EOS is met


                //--------------------------------------------------------------
                //
                // scanner construction
                //
                //--------------------------------------------------------------
                void          nothing(const Token &) const throw();  //!< ...
                void          newLine(const Token &) throw();        //!< send newLine to current source

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
                const Rule & forward(const LABEL   &anyLabel,
                                     const REGEXP  &anyRegExp,
                                     OBJECT_POINTER hObject,
                                     METHOD_POINTER hMethod)
                {
                    return regular<LABEL,REGEXP,OBJECT_POINTER,METHOD_POINTER,OnForward>(anyLabel,anyRegExp,hObject,hMethod);
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
                const Rule & discard(const LABEL   &anyLabel,
                                     const REGEXP  &anyRegExp,
                                     OBJECT_POINTER hObject,
                                     METHOD_POINTER hMethod)
                {
                    return regular<LABEL,REGEXP,OBJECT_POINTER,METHOD_POINTER,OnDiscard>(anyLabel,anyRegExp,hObject,hMethod);
                }

                //! default emit
                template <typename LABEL, typename REGEXP>
                const Rule & emit(const LABEL  &label, const REGEXP &regexp)
                {
                    return forward(label,regexp,this,&Scanner::nothing);
                }

                //! default drop
                template <typename LABEL, typename REGEXP>
                const Rule & drop(const LABEL  &label, const REGEXP &regexp)
                {
                    return discard(label,regexp,this,&Scanner::nothing);
                }

                //! default endl
                template <typename LABEL, typename REGEXP>
                const Rule &endl(const LABEL &label, const REGEXP &regexp)
                {
                    return discard(label,regexp,this,&Scanner::newLine);
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
                const Rule &call(const LABEL   &target,
                                 const REGEXP  &regexp,
                                 OBJECT_POINTER hObject,
                                 METHOD_POINTER hMethod)
                {
                    return leap<LABEL,REGEXP,OBJECT_POINTER,METHOD_POINTER,OnCall>(target,regexp,hObject,hMethod);
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
                const Rule &jump(const LABEL   &target,
                                 const REGEXP  &regexp,
                                 OBJECT_POINTER hObject,
                                 METHOD_POINTER hMethod)
                {
                    return leap<LABEL,REGEXP,OBJECT_POINTER,METHOD_POINTER,OnJump>(target,regexp,hObject,hMethod);
                }

                //! build a back event
                /**
                 - action is taken
                 - lexer shall go back to previous scanner
                 - different regexp may come back
                 */
                template <
                typename REGEXP,
                typename OBJECT_POINTER,
                typename METHOD_POINTER
                >
                const Rule &back(const REGEXP  &regexp,
                                 OBJECT_POINTER hObject,
                                 METHOD_POINTER hMethod)
                {
                    string              id = "<-";
                    id += regexp;
                    const Tag           ruleLabel = Tags::Make(id);
                    const Motif         ruleMotif = RegExp(regexp,dict);
                    const Action        ruleAction(hObject,hMethod);
                    const Event::Handle ruleEvent = new OnBack(ruleAction,label);
                    return add( new Rule(ruleLabel,ruleMotif,ruleEvent) );
                }

                //! create the call to a plugin with its Initialize method
                const Rule &call(Plugin &);

                //! query rule by name
                template <typename ID>
                const Rule * queryRule(const ID &id)
                {
                    const Rule **ppR = hoard.search(id);
                    return ppR ? *ppR : NULL;
                }


                //------------------------------------------------------------------
                //
                //! the flex-like probing function
                //
                /**
                 - if != NULL: a unit is produced
                 - if NULL   :
                 -- if Directive==NULL, EOF
                 -- otherwise follow directive!
                 */
                //------------------------------------------------------------------
                Unit *probe(Source &, Directive &);



                const Rules   rules; //!< list of rules
                const RulesDB hoard; //!< database of rules
                const Leading intro; //!< all possible first chars

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Scanner);
                Scatter       table;  //!< table of rules


                // create a regular rule
                template <
                typename LABEL,
                typename REGEXP,
                typename OBJECT_POINTER,
                typename METHOD_POINTER,
                typename REGULAR>
                const Rule &regular(const LABEL         &anyLabel,
                                    const REGEXP        &anyRegExp,
                                    const OBJECT_POINTER hObject,
                                    const METHOD_POINTER hMethod)
                {
                    assert(hObject);
                    assert(hMethod);
                    const Tag            ruleLabel = Tags::Make(anyLabel);
                    const Motif          ruleMotif = RegExp(anyRegExp,dict);
                    const Action         ruleAction(hObject,hMethod);
                    const Event::Handle  ruleEvent  = new REGULAR(ruleAction);
                    return add( new Rule(ruleLabel,ruleMotif,ruleEvent) );
                }

                template <
                typename LABEL,
                typename REGEXP,
                typename OBJECT_POINTER,
                typename METHOD_POINTER,
                typename LEAP
                >
                const Rule & leap(const LABEL   &target,
                                  const REGEXP  &regexp,
                                  OBJECT_POINTER hObject,
                                  METHOD_POINTER hMethod)
                {
                    const Tag            ruleLabel = Tags::Make(target);
                    const Motif          ruleMotif = RegExp(regexp,dict);
                    const Action         ruleAction(hObject,hMethod);
                    const Event::Handle  ruleEvent = new LEAP(ruleAction,ruleLabel);
                    return add( new Rule(ruleLabel,ruleMotif,ruleEvent) );
                }

                Unit     *endOfStream(const Source &) const;
                Unit     *tokenToUnit(Token  &, const Tag &) const;
                exception syntaxError(Source &) const;

            protected:
                Source                  *origin; //!< currently processed source
            public:
                const size_t             lmax;   //!< max rule label length
                const Dictionary * const dict;   //!< shared dictionary
            };
        }
    }

}

#endif

