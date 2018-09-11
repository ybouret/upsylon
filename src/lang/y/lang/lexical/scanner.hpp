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

            class Translator;


            //! simple 'Flex' scanner
            class Scanner : public CountedObject
            {
            public:
                typedef intr_ptr<string,Scanner> Pointer;
                
                const Origin label;

                //! for Pointer
                const string & key() const throw();

                //! destructor
                virtual ~Scanner() throw();
                explicit Scanner(const string &id) throw();
                explicit Scanner(const char   *id) throw();


                //! basic rule creation
                void add(const string          &rule_label,
                         const Pattern::Handle &rule_motif,
                         const Action          &the_action);

                //! probe the next unit
                /**
                 - if return NULL:
                 -- kind=forward: EOF
                 -- kind=control: change in translator
                 - if return not NULL, kind must be forward
                 */
                Unit *probe( Source &source, Result &result );

                template <typename OBJECT_POINTER, typename METHOD_POINTER>
                inline void make(const string     &id,
                                 const string     &rx,
                                 OBJECT_POINTER    host,
                                 METHOD_POINTER    meth)
                {
                    const Action          a(host,meth);
                    const Pattern::Handle m(Compile::RegExp(rx,userDict));
                    add(id,m,a);
                }

                template <typename OBJECT_POINTER, typename METHOD_POINTER>
                inline void make(const string     &rx,
                                 OBJECT_POINTER    host,
                                 METHOD_POINTER    meth)
                {
                    make(rx,rx,host,meth);
                }

                template <typename OBJECT_POINTER, typename METHOD_POINTER>
                inline void make(const char       *id,
                                 const char       *rx,
                                 OBJECT_POINTER    host,
                                 METHOD_POINTER    meth)
                {
                    const string _=id; const string __=rx; return make(_,__,host,meth);
                }

                template <typename OBJECT_POINTER, typename METHOD_POINTER>
                inline void make(const char       *rx,
                                 OBJECT_POINTER    host,
                                 METHOD_POINTER    meth)
                {
                    make(rx,rx,host,meth);
                }

                inline Result forward_(const Token &) throw() { return Forward; }
                inline Result discard_(const Token &) throw() { return Discard; }
                inline Result newline_(const Token &) throw() { assert(probed); probed->newLine(); return Discard; }

                inline void emit(const char *id, const char *rx) { make(id,rx,this,&Scanner::forward_); }
                inline void drop(const char *id, const char *rx) { make(id,rx,this,&Scanner::discard_); }
                inline void endl(const char *id, const char *rx) { make(id,rx,this,&Scanner::newline_); }

                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Scanner);
                Rule::List    rules;
                const Module *probed;

            public:
                const Dictionary *userDict; //!< validity must be checked by user
            };

        }

    }
}

#endif
