
//! \file
#ifndef Y_JIVE_SYNTAX_GRAMMAR_INCLUDED
#define Y_JIVE_SYNTAX_GRAMMAR_INCLUDED 1

#include "y/jive/syntax/axiom.hpp"

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
                axioms(), registry()
                {
                }
                
                //______________________________________________________________
                //
                // setup methods
                //______________________________________________________________

                //! append axiom to list and registry
                void declare(Axiom *axiom);

                //! append axiom
                template <typename AXIOM> inline
                AXIOM & add( AXIOM *axiom )
                {
                    declare(axiom);
                    return *axiom;
                }

                //! check existence
                template <typename ID> inline
                const Axiom *query( const ID &id ) const throw()
                {
                    const Axiom::Pointer *_ = registry.search_by(id);
                    return _ ? *_ : NULL;
                }
                
                //! get the root axiom
                const Axiom *getRoot() const throw();

                //! set the root axiom
                void setRoot(const Axiom &axiom);

                //! build tree
                Node *run( Lexer &lexer, Source &source ) const;


                //! render grammar axioms
                void  graphViz(const string &fileName) const;

                //! helper
                ios::ostream & graphTag(ios::ostream &fp, const Axiom &axiom) const;

                //! validate
                void  validate() const;

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const Tag name; //!< shared name


                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Grammar);
                Axiom::List     axioms;
                Axiom::Registry registry;

            };
            
        }
        
    }
    
}

#endif
