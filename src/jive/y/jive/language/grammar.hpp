//! \file

#ifndef Y_LANG_GRAMMAR_INCLUDED
#define Y_LANG_GRAMMAR_INCLUDED 1

#include "y/jive/language/axiom/all.hpp"
#include "y/container/addresses.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Language
        {


            typedef addresses<const Axiom> Axioms; //!< handle list of axioms
            
            //! echo typedef into class
#define Y_LANG_GRAMMAR_ECHO(CLASS) typedef Language::CLASS CLASS

            //__________________________________________________________________
            //
            //
            //! Grammar is a list of axioms
            //
            //__________________________________________________________________
            class Grammar
            {
            public:
                Y_LANG_GRAMMAR_ECHO(Axiom);       //!< alias
                Y_LANG_GRAMMAR_ECHO(Axioms);      //!< alias
                Y_LANG_GRAMMAR_ECHO(Aggregate);   //!< alias
                Y_LANG_GRAMMAR_ECHO(Alternate);   //!< alias
                Y_LANG_GRAMMAR_ECHO(Terminal);    //!< alias
                Y_LANG_GRAMMAR_ECHO(Compound);    //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                //! setup
                template <typename ID> inline
                explicit Grammar(const ID &id) :
                name( Tags::Make(id) ),
                axioms(),
                registry(),
                aligned(0),
                observer(name)
                {
                    observer.reserve(16);
                }

                //! cleanup
                virtual ~Grammar() throw();

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! axiom is pushed at the back of the axioms and registerd
                void declare(Axiom *axiom);

                //! helper
                template <typename AXIOM> inline
                AXIOM & add(AXIOM *axiom)
                {
                    assert(axiom);
                    declare(axiom);
                    return *axiom;
                }

                //! set Root axiom
                void setRoot(const Axiom &axiom);

                //! get Root axiom
                const Axiom * getRoot() const throw();

                //! query by name
                template <typename ID> inline
                const Axiom * query(const ID &id) const throw()
                {
                    const Axiom * const *ptr = registry.search(id);
                    return ptr ? *ptr : NULL;
                }

                //! query terminal and terminal type
                Terminal::Type lexemeType(const Lexeme *lx) const;

                //! find out if need to print content
                bool isStandard(const Lexeme *lx) const;

                //! test ownersip
                bool owns(const Axiom &axiom) const throw();
                
                //! try to validate
                void validateWith(const Lexer *) const;

                //______________________________________________________________
                //
                // terminal
                //______________________________________________________________

                //! a new terminal
                template <typename ID>
                const Terminal & term(const ID            &i,
                                      const Terminal::Type t)
                {
                    return add( new Terminal(i,t) );
                }

                //______________________________________________________________
                //
                // repeat
                //______________________________________________________________

                //! create a named repeat
                template <typename ID>
                const Repeat & rep(const ID    &id,
                                   const Axiom &axiom,
                                   const size_t atLeast)
                {
                    return add( new Repeat(id,axiom,atLeast,false) );
                }

                //! create an automatic repeat
                const Repeat & rep(const Axiom &axiom,
                                   const size_t atLeast);

                const Repeat & zeroOrMore(const Axiom &); //!< rep(axiom,0)
                const Repeat & oneOrMore(const Axiom  &); //!< rep(axiom,1)


                //______________________________________________________________
                //
                // option
                //______________________________________________________________

                //! create a named option
                template <typename ID>
                const Option & opt(const ID &id, const Axiom &axiom)
                {
                    return add( new Option(id,axiom) );
                }

                //! create an automatic option
                const Option & opt(const Axiom &axiom);

                //______________________________________________________________
                //
                // aggregate
                //______________________________________________________________

                //! named aggregate
                template <typename ID>
                Aggregate & agg(const ID &id) {
                    return add( new Aggregate(id,Aggregate::Standard) );
                }

                //! named grouping aggregate
                template <typename ID>
                Aggregate & grp(const ID &id) {
                    return add( new Aggregate(id,Aggregate::Grouping) );
                }

                //! named variadic aggregate
                template <typename ID>
                Aggregate & var(const ID &id) {
                    return add( new Aggregate(id,Aggregate::Variadic) );
                }
                
                //! grouping of axioms
                const Axiom & cat(const Axioms &);
                
                //______________________________________________________________
                //
                // alternate
                //______________________________________________________________
                
                //! named alternate
                template <typename ID>
                Alternate & alt(const ID &id) {
                    return add( new Alternate(id) );
                }
                
                //! automatic alternate
                const Axiom &pick(const Axioms &);
                
                //______________________________________________________________
                //
                // running on source
                //______________________________________________________________

                //! accept the root Axiom
                XNode *run(Source &source, Lexer &lexer);
                
                //! output graphViz
                void   graphViz(const string &fileName) const;
                
                //! outpt graphViz, wrapper
                void   graphViz(const char   *fileName) const;

                
                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const Tag             name;      //!< shared name
                const Axiom::List     axioms;    //!< axioms
                const Axiom::Registry registry;  //!< registry
                const size_t          aligned;   //!< max of axioms name length

            private:
                Observer              observer;

                Y_DISABLE_COPY_AND_ASSIGN(Grammar);
                string makeAxiomsID(const Axioms &ax,
                                    const char   *fn,
                                    const char    sep) const;

                XNode *onAccept(XNode *node, Source &source, Lexer &lexer) const;
                XNode *onReject(Source &source, Lexer &lexer) const;

            };
        }
    }
}

#endif

