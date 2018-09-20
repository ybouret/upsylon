
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
        ast(0),
        cst(0)
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
            cst = 0;

            {
                auto_ptr<NODE> AST = parser->parse(source, 0 != (flags&KeepRaw) );
                if(0!=(flags&KeepAST))
                {
                    ast = new NODE( *AST );
                }


                cst = NODE::AST( NODE::Rewrite( &*AST, *(this->name)) );
                AST.dismiss();
            }
            

            walk(*cst);

        }


    }

}
