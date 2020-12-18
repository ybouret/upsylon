

//! \file

#ifndef Y_JIVE_DIALECT_COMPILER_INCLUDED
#define Y_JIVE_DIALECT_COMPILER_INCLUDED 1

#include "y/jive/dialect/parser.hpp"
#include "y/jive/syntax/analyzer.hpp"

namespace upsylon
{

    namespace Jive
    {

        namespace Dialect
        {
            //! todo
            class PreAnalyzer : public Syntax::Analyzer
            {
            public:
                //! setup
                template <typename ID> inline
                explicit PreAnalyzer( const ID &id, Jive::Parser *p) :
                Syntax::Analyzer( id ),
                parser(p)
                {
                    assert(parser!=NULL);
                }

                //! cleanup
                virtual ~PreAnalyzer() throw();

                Jive::Parser *parser; //!< linked

            private:
                Y_DISABLE_COPY_AND_ASSIGN(PreAnalyzer);
            };

            //__________________________________________________________________
            //
            //
            //! compile a parsed grammar description
            //
            //__________________________________________________________________
            class Compiler
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit Compiler();          //!< setup
                virtual ~Compiler() throw();  //!< cleanup

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! full compilation
                Jive::Parser *compileAST( XNode::Pointer &ast );
            
                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const Tag   name; //!< Dialect::Compiler
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Compiler);
                const string *GetName( XNode::Pointer &ast ) const;

            };
        }

    }

}

#endif
