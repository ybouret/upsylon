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

            //------------------------------------------------------------------
            //
            //! simple 'Flex' scanner
            /**
             Events are triggered upon pattern recognition
             */
            //------------------------------------------------------------------
            class Scanner : public CountedObject
            {
            public:
                typedef intr_ptr<string,Scanner> Pointer; //!< for database

                const Tag label; //!< used as shared label/key

                virtual ~Scanner() throw();                 //!< destructor
                explicit Scanner(const string &id);         //!< initialize, create a label
                explicit Scanner(const char   *id);         //!< initialize, create a label
                explicit Scanner(const Tag    &id) throw(); //!< initialize, shared label
                const string & key() const throw();         //!< for database

                //! construct a new rule, for any EVENT, and a Motif that must NOT be weak
                template <typename EVENT>
                inline void add(const Tag         &ruleLabel,
                                const Motif       &ruleMotif,
                                const EVENT       &ruleEvent)
                {
                    if(verbose) emitLabel(ruleLabel);
                    checkLabel(ruleLabel);
                    checkMotif(ruleLabel,ruleMotif);
                    rules.push_back( new Rule(ruleLabel,ruleMotif,ruleEvent) );
                }

                //! construct a forwading rule
                /**
                 upon recognition or rx=token, host.meth(token) is called
                 and the token will be emitted as a regular lexeme
                 */
                template <typename OBJECT_POINTER, typename METHOD_POINTER>
                inline void forward(const string   &id,
                                    const string   &rx,
                                    OBJECT_POINTER  host,
                                    METHOD_POINTER  meth)
                {
                    const Tag          ruleLabel = new string(id);
                    const Motif        ruleMotif = RegExp(rx,userDict);
                    const Action       ruleAction(host,meth);
                    const RegularCode  ruleEvent = new OnForward(ruleAction);
                    add(ruleLabel,ruleMotif,ruleEvent);
                }

                //! construct a forwarding rule, wrapper
                template <typename OBJECT_POINTER, typename METHOD_POINTER>
                inline void forward(const char     *id,
                                    const char     *rx,
                                    OBJECT_POINTER  host,
                                    METHOD_POINTER  meth)
                {
                    const string _ = id; const string __ = rx; forward(_,__,host,meth);
                }

                //! construct a discarding rule
                /**
                 upon recognition or rx=token, host.meth(token) is called
                 and the token is discarded, and the source is probed again
                 */
                template <typename OBJECT_POINTER, typename METHOD_POINTER>
                inline void discard(const string   &id,
                                    const string   &rx,
                                    OBJECT_POINTER  host,
                                    METHOD_POINTER  meth)
                {
                    const Tag          ruleLabel = new string(id);
                    const Motif        ruleMotif = RegExp(rx,userDict);
                    const Action       ruleAction(host,meth);
                    const RegularCode  ruleEvent = new OnDiscard(ruleAction);
                    add(ruleLabel,ruleMotif,ruleEvent);
                }

                //! construct a discarding rule, wrapper
                template <typename OBJECT_POINTER, typename METHOD_POINTER>
                inline void discard(const char     *id,
                                    const char     *rx,
                                    OBJECT_POINTER  host,
                                    METHOD_POINTER  meth)
                {
                    const string _ = id; const string __ = rx; discard(_,__,host,meth);
                }

                //! make a jump label to target
                static string JumpLabel( const string &target );

                //! construct a jump
                /**
                 upon recognition of rx=token, host.method(token) is called
                 and another target scanner is set as active,
                 without memory of the calling scanner.
                 Then the source will be probed again.
                 */
                template <typename OBJECT_POINTER, typename METHOD_POINTER>
                void jump(const string   &target,
                          const string   &rx,
                          OBJECT_POINTER  host,
                          METHOD_POINTER  meth)
                {
                    const string       id        = JumpLabel(target);
                    const Tag          ruleLabel = new string(id);
                    const Motif        ruleMotif = RegExp(rx,userDict);
                    const Action       ruleAction(host,meth);
                    const ControlCode  ruleEvent = new OnJump(target,ruleAction);
                    add(ruleLabel,ruleMotif,ruleEvent);
                }

                //! construct a jump, wrapper
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
                /**
                 upon recognition of rx=token, host.method(token) is called
                 and another target scanner is set as active,
                 and the current scanner is pushed onto the history stack.
                 Then the source will be probed again.
                 */
                template <typename OBJECT_POINTER, typename METHOD_POINTER>
                void call(const string   &target,
                          const string   &rx,
                          OBJECT_POINTER  host,
                          METHOD_POINTER  meth)
                {
                    const string       id        = CallLabel(target);
                    const Tag          ruleLabel = new string(id);
                    const Motif        ruleMotif = RegExp(rx,userDict);
                    const Action       ruleAction(host,meth);
                    const ControlCode  ruleEvent = new OnCall(target,ruleAction);
                    add(ruleLabel,ruleMotif,ruleEvent);
                }

                //! construct a  call, wrapper
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

                //! just call to target on a regular expression, wrapper
                inline void call(const char *target, const char *rx)
                {
                    const string _ = target; const string __ = rx; call(_,__);
                }


                //! create a BackLabel from the label and the regular expression
                static  string BackLabel( const string &l, const string &rx );

                //! construct a come back rule
                /**
                 upon recognition of rx=token, host.method(token) is called
                 and the last scanner in history if set as active.
                 Then the source will be probed again.
                 */
                template <typename OBJECT_POINTER, typename METHOD_POINTER>
                void back(const string   &rx,
                          OBJECT_POINTER  host,
                          METHOD_POINTER  meth)
                {
                    const string       id        = BackLabel(*label,rx);
                    const Tag          ruleLabel = new string(id);
                    const Motif        ruleMotif = RegExp(rx,userDict);
                    const Action       ruleAction(host,meth);
                    const ControlCode  ruleEvent = new OnBack(ruleAction);
                    add(ruleLabel,ruleMotif,ruleEvent);
                }

                //! construct a come back, wrapper
                template <typename OBJECT_POINTER, typename METHOD_POINTER>
                inline void back(const char     *rx,
                                 OBJECT_POINTER  host,
                                 METHOD_POINTER  meth)
                {
                    const string __ = rx; back(__,host,meth);
                }

                //! return without any further ado
                inline void back(const string &rx) { back(rx,this,&Scanner::nothing); }

                //! return without any further ado
                inline void back(const char *rx) { const string _(rx); back(_); }

                //! do nothing
                void nothing(const Token &) throw();

                //! newline of the probed source
                void newline(const Token &) throw();

                //! helper to emit ID on rx
                void emit(const string &id,const string &rx);

                //! helper to emit ID on rx
                void emit(const char   *id,const char   *rx);

                //! helper to drop ID on rx
                void drop(const string &id,const string &rx);

                //! helper to drop ID on rx
                void drop(const char   *id,const char   *rx);

                //! helper to drop rx on rx
                void drop(const char *rx);

                //! helper for newline(id) on rx
                void endl(const string &id,const string &rx);

                //! helper for newline(id) on rx
                void endl(const char   *id,const char   *rx);
                
                //! helper for endl shortcut
                void endl(const char *rx);

                //! probe source
                /**
                 - return NULL: if no event, EOF, otherwise control for translator
                 - otherwise result of a forwarding rule
                 */
                Lexeme *probe( Source &source, Message &msg );

                //! find a rule by its label
                const Rule * getRuleByLabel( const string &id ) const throw();

                //! return the last added rule
                const Rule &last() const throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Scanner);
                Rule::List    rules;
                const Module *probed;
                void checkLabel(const Tag &ruleLabel) const;
                void checkMotif(const Tag &ruleLabel, const Motif &ruleMotif) const;
                void emitLabel(const Tag &ruleLabel) const;

            public:
                const Dictionary *userDict; //!< validity must be checked by user
                bool              verbose;  //!< to check build up of the scanner
                
                //! indent using depth
                std::ostream     &indent( std::ostream &) const;
            };

        }

    }
}

#endif
