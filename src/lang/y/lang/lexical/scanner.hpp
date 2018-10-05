//! \file
#ifndef Y_LANG_LEXICAL_SCANNER_INCLUDED
#define Y_LANG_LEXICAL_SCANNER_INCLUDED 1

#include "y/lang/source.hpp"
#include "y/lang/lexical/rule.hpp"
#include "y/lang/pattern/compiler.hpp"
#include "y/ptr/intr.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {
            typedef const ControlEvent *Message; //!< alias for probe

            //! simple 'Flex' scanner
            class Scanner : public CountedObject
            {
            public:
                typedef intr_ptr<string,Scanner> Pointer; //!< for database

                const Origin label; //!< used as shared label/key


                virtual ~Scanner() throw();                 //!< destructor
                explicit Scanner(const string &id);         //!< initialize
                explicit Scanner(const char   *id);         //!< initialize
                explicit Scanner(const Origin &id) throw(); //!< initialize
                const string & key() const throw();         //!< for database

                //! construct a new rule
                template <typename CODE>
                inline void add(const Origin      &ruleLabel,
                                const Motif       &ruleMotif,
                                const CODE        &ruleEvent)
                {
                    if(verbose) { std::cerr << "@scan[" << label << "].add( '" << ruleLabel << "' )" << std::endl; }
                    checkLabel(ruleLabel);
                    rules.push_back( new Rule(ruleLabel,ruleMotif,ruleEvent) );
                }

                //! construct a forwading rule
                template <typename OBJECT_POINTER, typename METHOD_POINTER>
                inline void forward(const string   &id,
                                    const string   &rx,
                                    OBJECT_POINTER  host,
                                    METHOD_POINTER  meth)
                {
                    const Origin       ruleLabel = new string(id);
                    const Motif        ruleMotif = RegExp(rx,userDict);
                    const Action       ruleAction(host,meth);
                    const RegularCode  ruleEvent = new OnForward(ruleAction);
                    add(ruleLabel,ruleMotif,ruleEvent);
                }

                //! construct a forwarding rule
                template <typename OBJECT_POINTER, typename METHOD_POINTER>
                inline void forward(const char     *id,
                                    const char     *rx,
                                    OBJECT_POINTER  host,
                                    METHOD_POINTER  meth)
                {
                    const string _ = id; const string __ = rx; forward(_,__,host,meth);
                }

                //! construct a discarding rule
                template <typename OBJECT_POINTER, typename METHOD_POINTER>
                inline void discard(const string   &id,
                                    const string   &rx,
                                    OBJECT_POINTER  host,
                                    METHOD_POINTER  meth)
                {
                    const Origin       ruleLabel = new string(id);
                    const Motif        ruleMotif = RegExp(rx,userDict);
                    const Action       ruleAction(host,meth);
                    const RegularCode  ruleEvent = new OnDiscard(ruleAction);
                    add(ruleLabel,ruleMotif,ruleEvent);
                }

                //! construct a discarding rule
                template <typename OBJECT_POINTER, typename METHOD_POINTER>
                inline void discard(const char     *id,
                                    const char     *rx,
                                    OBJECT_POINTER  host,
                                    METHOD_POINTER  meth)
                {
                    const string _ = id; const string __ = rx; discard(_,__,host,meth);
                }

                //! construct a jump
                template <typename OBJECT_POINTER, typename METHOD_POINTER>
                void jump(const string   &target,
                          const string   &rx,
                          OBJECT_POINTER  host,
                          METHOD_POINTER  meth)
                {
                    const string       id        = "->" + target;
                    const Origin       ruleLabel = new string(id);
                    const Motif        ruleMotif = RegExp(rx,userDict);
                    const Action       ruleAction(host,meth);
                    const ControlCode  ruleEvent = new OnJump(target,ruleAction);
                    add(ruleLabel,ruleMotif,ruleEvent);
                }

                //! construct a jump
                template <typename OBJECT_POINTER, typename METHOD_POINTER>
                inline void jump(const char     *target,
                                 const char     *rx,
                                 OBJECT_POINTER  host,
                                 METHOD_POINTER  meth)
                {
                    const string _ = target; const string __ = rx; jump(_,__,host,meth);
                }

                //! just jump to target on a regular expression
                inline void jump(const string &target, const string &rx)
                {
                    jump(target,rx,this,&Scanner::nothing);
                }

                //! just jump to target on a regular expression
                inline void jump(const char *target, const char *rx)
                {
                    const string _ = target; const string __ = rx; jump(_,__);
                }

                //! make the call label
                static string CallLabel( const string &target );

                //! construct a call
                template <typename OBJECT_POINTER, typename METHOD_POINTER>
                void call(const string   &target,
                          const string   &rx,
                          OBJECT_POINTER  host,
                          METHOD_POINTER  meth)
                {
                    const string       id        = CallLabel(target);
                    const Origin       ruleLabel = new string(id);
                    const Motif        ruleMotif = RegExp(rx,userDict);
                    const Action       ruleAction(host,meth);
                    const ControlCode  ruleEvent = new OnCall(target,ruleAction);
                    add(ruleLabel,ruleMotif,ruleEvent);
                }

                //! construct a  call
                template <typename OBJECT_POINTER, typename METHOD_POINTER>
                inline void call(const char     *target,
                                 const char     *rx,
                                 OBJECT_POINTER  host,
                                 METHOD_POINTER  meth)
                {
                    const string _ = target; const string __ = rx; call(_,__,host,meth);
                }

                //! just call to target on a regular expression
                inline void call(const string &target, const string &rx)
                {
                    call(target,rx,this,&Scanner::nothing);
                }

                //! just call to target on a regular expression
                inline void call(const char *target, const char *rx)
                {
                    const string _ = target; const string __ = rx; call(_,__);
                }


                //! construct a back
                template <typename OBJECT_POINTER, typename METHOD_POINTER>
                void back(const string   &rx,
                          OBJECT_POINTER  host,
                          METHOD_POINTER  meth)
                {
                    const string       id        = "<=" + *label + "." + rx;
                    const Origin       ruleLabel = new string(id);
                    const Motif        ruleMotif = RegExp(rx,userDict);
                    const Action       ruleAction(host,meth);
                    const ControlCode  ruleEvent = new OnBack(ruleAction);
                    add(ruleLabel,ruleMotif,ruleEvent);
                }

                //! construct a back
                template <typename OBJECT_POINTER, typename METHOD_POINTER>
                inline void back(const char     *rx,
                                 OBJECT_POINTER  host,
                                 METHOD_POINTER  meth)
                {
                    const string __ = rx; back(__,host,meth);
                }

                //! return without any further ado
                inline void back(const string &rx)
                {
                    back(rx,this,&Scanner::nothing);
                }

                //! return without any further ado
                inline void back(const char *rx)
                {
                    const string _(rx); back(_);
                }

                //! do nothing
                inline void nothing( const Token &) throw() {}
                //! newline of the probed source
                inline void newline(const Token &) throw() { if(probed) probed->newLine(); }

                //! helper to emit ID on rx
                inline void emit(const string &id,const string &rx) { forward(id,rx,this,&Scanner::nothing); }

                //! helper to emit ID on rx
                inline void emit(const char   *id,const char   *rx) { const string _=id; const string __=rx; emit(_,__); }

                //! helper to drop ID on rx
                inline void drop(const string &id,const string &rx) { discard(id,rx,this,&Scanner::nothing); }

                //! helper to drop ID on rx
                inline void drop(const char   *id,const char   *rx) { const string _=id; const string __=rx; drop(_,__); }

                //! helper to drop rx on rx
                inline void drop(const char *rx) { const string _(rx); drop(_,_); }


                //! helper for newline(id) on rx
                inline void endl(const string &id,const string &rx) { discard(id,rx,this,&Scanner::newline); }

                //! helper for newline(id) on rx
                inline void endl(const char   *id,const char   *rx) { const string _=id; const string __=rx; endl(_,__); }

                //! helper for endl shortcut
                inline void endl(const char *rx) { const string _(rx); endl(_,_); }
                

                //! probe source
                /**
                 - return NULL: if no event, EOF, otherwise control for translator
                 - otherwise result of a forwarding rule
                 */
                Lexeme *probe( Source &source, Message &msg );

                //! find a rule by its label
                const Rule * getRuleByLabel( const string &id ) const throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Scanner);
                Rule::List    rules;
                const Module *probed;
                void checkLabel(const Origin &ruleLabel) const;

            public:
                const Dictionary *userDict; //!< validity must be checked by user
                bool              verbose;
            };

        }

    }
}

#endif
