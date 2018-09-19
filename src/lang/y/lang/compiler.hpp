//! \file
#ifndef Y_LANG_SYNTAX_COMPILER_INCLUDED
#define Y_LANG_SYNTAX_COMPILER_INCLUDED 1

#include "y/lang/syntax/analyzer.hpp"
#include "y/lang/syntax/parser.hpp"

namespace upsylon
{
    namespace Lang
    {
        class Compiler : public Syntax::Analyzer
        {
        public:
            explicit Compiler(Syntax::Parser *p) throw();
            virtual ~Compiler() throw();

            void compile( Module *module );

        protected:
            Syntax::Parser::Pointer parser;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Compiler);
            
        public:
            const Origin name;
        };
    }
}

#endif

