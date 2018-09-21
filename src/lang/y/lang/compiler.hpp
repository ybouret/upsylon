//! \file
#ifndef Y_LANG_SYNTAX_COMPILER_INCLUDED
#define Y_LANG_SYNTAX_COMPILER_INCLUDED 1

#include "y/lang/syntax/analyzer.hpp"
#include "y/lang/syntax/parser.hpp"

namespace upsylon
{
    namespace Lang
    {
        //! a compiler is an analyzer with a parser
        class Compiler : public Syntax::Analyzer
        {
        public:
            typedef Syntax::Node  NODE;           //!< alias

            //! initialize
            explicit Compiler(Syntax::Parser *p) throw();
            //! destructor
            virtual ~Compiler() throw();

            //! get the AST, rewrite it and walk it down
            void compile( Module *module, bool keepRaw = false );

            //! helper to access parser
            const Syntax::Parser * operator->() const throw();

            //! helper to access parser
            Syntax::Parser * operator->() throw();

        protected:
            Syntax::Parser::Pointer parser; //!< a compiled parser

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Compiler);
            
        public:
            const Origin           name; //!< Grammar::name
            auto_ptr<NODE>         ast;  //!< last parsed AST
        };


    }
}

#endif

