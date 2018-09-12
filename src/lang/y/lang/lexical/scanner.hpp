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

            //! simple 'Flex' scanner
            class Scanner : public CountedObject
            {
            public:
                typedef intr_ptr<string,Scanner> Pointer; //! for database
                const Origin label; //!< used as shared label/key


                virtual ~Scanner() throw();                 //!< destructor
                explicit Scanner(const string &id) throw(); //!< initialize
                explicit Scanner(const char   *id) throw(); //!< initialize
                const string & key() const throw();         //!< for database

                //! construct a new rule
                template <typename CODE>
                inline void add(const Origin      &ruleLabel,
                                const Motif       &ruleMotif,
                                const CODE        &ruleEvent)
                {
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
                    const Motif        ruleMotif = Compile::RegExp(rx,userDict);
                    const Action       ruleAction(host,meth);
                    const RegularCode  ruleEvent = new OnForward(ruleAction);
                    add(ruleLabel,ruleMotif,ruleEvent);
                }

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
                    const Motif        ruleMotif = Compile::RegExp(rx,userDict);
                    const Action       ruleAction(host,meth);
                    const RegularCode  ruleEvent = new OnDiscard(ruleAction);
                    add(ruleLabel,ruleMotif,ruleEvent);
                }

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
                    const string       id        = "jump@" + target;
                    const Origin       ruleLabel = new string(id);
                    const Motif        ruleMotif = Compile::RegExp(rx,userDict);
                    const Action       ruleAction(host,meth);
                    const ControlCode  ruleEvent = new OnJump(target,ruleAction);
                    add(ruleLabel,ruleMotif,ruleEvent);
                }

                template <typename OBJECT_POINTER, typename METHOD_POINTER>
                inline void jump(const char     *target,
                                 const char     *rx,
                                 OBJECT_POINTER  host,
                                 METHOD_POINTER  meth)
                {
                    const string _ = target; const string __ = rx; jump(_,__,host,meth);
                }

                //! construct a call
                template <typename OBJECT_POINTER, typename METHOD_POINTER>
                void call(const string   &target,
                          const string   &rx,
                          OBJECT_POINTER  host,
                          METHOD_POINTER  meth)
                {
                    const string       id        = "call@" + target;
                    const Origin       ruleLabel = new string(id);
                    const Motif        ruleMotif = Compile::RegExp(rx,userDict);
                    const Action       ruleAction(host,meth);
                    const ControlCode  ruleEvent = new OnCall(target,ruleAction);
                    add(ruleLabel,ruleMotif,ruleEvent);
                }

                template <typename OBJECT_POINTER, typename METHOD_POINTER>
                inline void call(const char     *target,
                                 const char     *rx,
                                 OBJECT_POINTER  host,
                                 METHOD_POINTER  meth)
                {
                    const string _ = target; const string __ = rx; call(_,__,host,meth);
                }

                //! construct a back
                template <typename OBJECT_POINTER, typename METHOD_POINTER>
                void back(const string   &rx,
                          OBJECT_POINTER  host,
                          METHOD_POINTER  meth)
                {
                    const string       id        = "back<" + *label;
                    const Origin       ruleLabel = new string(id);
                    const Motif        ruleMotif = Compile::RegExp(rx,userDict);
                    const Action       ruleAction(host,meth);
                    const ControlCode  ruleEvent = new OnBack(ruleAction);
                    add(ruleLabel,ruleMotif,ruleEvent);
                }

                //! back
                template <typename OBJECT_POINTER, typename METHOD_POINTER>
                inline void back(const char     *rx,
                                 OBJECT_POINTER  host,
                                 METHOD_POINTER  meth)
                {
                    const string __ = rx; back(__,host,meth);
                }

                inline void nothing( const Token &) throw() {}
                inline void newline(const Token &) throw() { assert(probed); probed->newLine(); }

                inline void emit(const string &id,const string &rx) { forward(id,rx,this,&Scanner::nothing); }
                inline void drop(const string &id,const string &rx) { discard(id,rx,this,&Scanner::nothing); }
                inline void endl(const string &id,const string &rx) { discard(id,rx,this,&Scanner::newline); }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Scanner);
                Rule::List    rules;
                const Module *probed;
                void checkLabel(const Origin &ruleLabel) const;

            public:
                const Dictionary *userDict; //!< validity must be checked by user
            };

        }

    }
}

#endif
