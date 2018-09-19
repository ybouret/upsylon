
#include "y/lang/compiler.hpp"

namespace upsylon
{
    namespace Lang
    {
        Compiler:: ~Compiler() throw()
        {
        }

        Compiler:: Compiler( Syntax::Parser *p ) throw() :
        Syntax::Analyzer(),
        parser(p),
        name(parser->name),
        ast(0)
        {
        }

        const Syntax::Parser * Compiler:: operator->() const throw()
        {
            return & *parser;
        }

        void Compiler:: compile( Module *module , const unsigned flags)
        {
            Source source(module);
            parser->reset();
            ast = 0;

            ast = parser->parse(source, 0 != (flags&KeepRaw) );
            walk( *ast );
            if( 0 == (flags&KeepAST) )
            {
                ast = 0;
            }
        }


    }

}
