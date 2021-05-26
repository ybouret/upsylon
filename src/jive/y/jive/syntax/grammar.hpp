
//! \file
#ifndef Y_JIVE_SYNTAX_GRAMMAR_INCLUDED
#define Y_JIVE_SYNTAX_GRAMMAR_INCLUDED 1

#include "y/jive/syntax/axiom/all.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {
            //__________________________________________________________________
            //
            //
            //! Grammar
            //
            //__________________________________________________________________
            class Grammar
            {
            public:
                //______________________________________________________________
                //
                // type and definitions
                //______________________________________________________________
                static const char BuiltIn = '#'; //!< for built-in namrs
                

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                //! cleanup
                virtual ~Grammar() throw();
                
                //! setup
                template <typename ID> inline
                explicit Grammar( const ID &id ) :
                name( Tags::Make(id) ),
                maxNameLength(0),
                axioms(),
                registry(),
                indxAggregate(1),
                indxAlternate(1)
                {
                }
                
                //______________________________________________________________
                //
                //  methods
                //______________________________________________________________

                //--------------------------------------------------------------
                //! append axiom to list and registry
                /**
                 axiom is fully handled
                 */
                //--------------------------------------------------------------
                void declare(Axiom *axiom);

                //--------------------------------------------------------------
                //! helper to append a derived axiom
                //--------------------------------------------------------------
                template <typename AXIOM> inline
                AXIOM & add( AXIOM *axiom )
                {
                    declare(axiom);
                    return *axiom;
                }

                //--------------------------------------------------------------
                //! check existence
                //--------------------------------------------------------------
                template <typename ID> inline
                const Axiom *query( const ID &id ) const throw()
                {
                    const Axiom::Pointer *_ = registry.search(id);
                    return _ ? *_ : NULL;
                }

                //--------------------------------------------------------------
                //! get the root axiom
                //--------------------------------------------------------------
                const Axiom *getRoot() const throw();

                //--------------------------------------------------------------
                //! set the root axiom
                //--------------------------------------------------------------
                void setRoot(const Axiom &axiom);


                //--------------------------------------------------------------
                //! build tree
                /**
                 try to accept root Axiom
                 */
                //--------------------------------------------------------------
                Node *run(Lexer &lexer, Source &source) const;

                //--------------------------------------------------------------
                //! render grammar
                //--------------------------------------------------------------
                void  graphViz(const string &fileName) const;

                //--------------------------------------------------------------
                //! helper to create node label
                //--------------------------------------------------------------
                ios::ostream & graphTag(ios::ostream &fp, const Axiom &axiom) const;

                //--------------------------------------------------------------
                //! validate grammar
                /**
                 ensure all nodes are properly connected
                 */
                //--------------------------------------------------------------
                void  validate() const;

                //--------------------------------------------------------------
                //
                // helper to create grammar
                //
                //--------------------------------------------------------------

                //--------------------------------------------------------------
                //! create a terminal
                //--------------------------------------------------------------
                template <typename ID>
                const Terminal & term(const ID            &i,
                                      const Terminal::Type k )
                {
                    return add( new Terminal(i,k) );
                }

                //--------------------------------------------------------------
                //! create a named end line
                //--------------------------------------------------------------
                template <typename ID>
                const EndLine & end_line(const ID &i)
                {
                    return add( new EndLine(i) );
                }



                //--------------------------------------------------------------
                //! create a named repeat
                //--------------------------------------------------------------
                template <typename ID>
                const Axiom & repeat(const ID &id, const Axiom &axiom, const size_t atLeast)
                {
                    return add( new Repeat(id,axiom,atLeast) );
                }

                //--------------------------------------------------------------
                //! create an automatic repeat
                //--------------------------------------------------------------
                const Axiom & repeat(const Axiom &axiom, const size_t atLeast);

                //--------------------------------------------------------------
                //! create an zero or more
                //--------------------------------------------------------------
                const Axiom & zeroOrMore(const Axiom &);

                //--------------------------------------------------------------
                //! create an automatic one or more
                //--------------------------------------------------------------
                const Axiom & oneOrMore(const Axiom &);

                //--------------------------------------------------------------
                //! create a named option
                //--------------------------------------------------------------
                template <typename ID>
                const Axiom & option(const ID &id, const Axiom &axiom )
                {
                    return add( new Option(id,axiom) );
                }

                //--------------------------------------------------------------
                //! create an automatic option
                //--------------------------------------------------------------
                const Axiom & option(const Axiom &axiom);

                //--------------------------------------------------------------
                //! create a named aggregate
                //--------------------------------------------------------------
                template <typename ID>
                Aggregate & agg(const ID &id)
                {
                    return add( new Aggregate(id,Aggregate::Standard) );
                }



                //--------------------------------------------------------------
                //! create a variadic aggregate, vanish iff one child only
                //--------------------------------------------------------------
                template <typename ID>
                Aggregate & var(const ID &id)
                {
                    return add( new Aggregate(id,Aggregate::Variadic) );
                }


                //--------------------------------------------------------------
                //! create a transient aggregate
                //--------------------------------------------------------------
                template <typename ID>
                Aggregate & grp(const ID &id)
                {
                    return add( new Aggregate(id,Aggregate::Grouping) );
                }

                //--------------------------------------------------------------
                //! create an automatic transient aggregate
                //--------------------------------------------------------------
                Aggregate & grp();
 



                //--------------------------------------------------------------
                //! create an automatic aggregate
                //--------------------------------------------------------------
                const Axiom & cat(const Axiom &a, const Axiom &b);
               
                //--------------------------------------------------------------
                //! create an automatic aggregate
                //--------------------------------------------------------------
                const Axiom & cat(const Axiom &a, const Axiom &b, const Axiom &c);

                
                //--------------------------------------------------------------
                //! create a named alternate
                //--------------------------------------------------------------
                template <typename ID>
                Alternate & alt(const ID &id)
                {
                    return add( new Alternate(id) );
                }
                
                //--------------------------------------------------------------
                //! create an automatic alternate
                //--------------------------------------------------------------
                Alternate & alt();

                //--------------------------------------------------------------
                //! create an automatic alternate
                //--------------------------------------------------------------
                const Axiom & choice(const Axiom &a, const Axiom &b);
                
                //--------------------------------------------------------------
                //! create an automatic alternate
                //--------------------------------------------------------------
                const Axiom & choice(const Axiom &a, const Axiom &b, const Axiom &c);

                //--------------------------------------------------------------
                //! query lexeme type
                //--------------------------------------------------------------
                Terminal::Type lexemeType(const Lexeme &lexeme) const;

                //--------------------------------------------------------------
                //! query internal registry of axioms
                //--------------------------------------------------------------
                const Axiom::Registry & getRegistry() const;

                //--------------------------------------------------------------
                //! query internal list of axioms
                //--------------------------------------------------------------
                const Axiom::List     & getList() const;
                
                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const Tag    name;          //!< shared name
                const size_t maxNameLength; //!< axioms max name length
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Grammar);
                Axiom::List     axioms;
                Axiom::Registry registry;
                unsigned        indxAggregate;
                unsigned        indxAlternate;
                Node           *processSuccess(Node *, const Observer &, Lexer &, Source &) const;
                void            processFailure(const Observer &, Lexer &, Source &) const;
            };
            
        }
        
    }
    
}

#endif
