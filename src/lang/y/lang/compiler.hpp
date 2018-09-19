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
            static const unsigned KeepRaw = 0x01; //!< will keep raw in parser
            static const unsigned KeepAST = 0x02; //!< will keep ast in compiler
            typedef Syntax::Node  NODE;           //!< alias

            //! initialize
            explicit Compiler(Syntax::Parser *p) throw();
            //! destructor
            virtual ~Compiler() throw();

            //! get the AST and walk it down
            void compile( Module *module, const unsigned flags = 0 );

            //! helper to access parser->raw
            const Syntax::Parser * operator->() const throw();

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

