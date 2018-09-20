
#include "y/lang/compiler.hpp"
#include "y/exception.hpp"

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
                bool needRewrite = false;
                auto_ptr<NODE> AST = parser->parse(source, needRewrite, 0 != (flags&KeepRaw) );
                if(0!=(flags&KeepAST))
                {
                    ast = new NODE( *AST );
                }

                if(needRewrite)
                {
                    needRewrite = false;
                    cst = NODE::AST( NODE::Rewrite( &*AST, *(this->name)), &needRewrite);
                    if(needRewrite)
                    {
                        throw exception("{%s} still need to re-write tree!", **(parser->name));
                    }
                    AST.dismiss();
                }
                else
                {
                    cst = AST.yield();
                }
            }
            

            walk(*cst);

        }


    }

}
