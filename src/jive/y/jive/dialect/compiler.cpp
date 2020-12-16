
#include "y/jive/dialect/compiler.hpp"

namespace upsylon
{

    namespace Jive
    {

        namespace Dialect
        {
          
            PreAnalyzer:: ~PreAnalyzer() throw()
            {

            }
        }

    }
}

namespace upsylon
{

    namespace Jive
    {

        namespace Dialect
        {

            Compiler:: Compiler() : name( Tags::Make("Dialect::Compiler") )
            {
                
            }

            Compiler:: ~Compiler() throw()
            {
                
            }


            
            Jive::Parser * Compiler:: compileAST( XNode::Pointer &ast )
            {
                //--------------------------------------------------------------
                //
                // check
                //
                //--------------------------------------------------------------
                if( !ast->isInternal( Parser::DialectID ) ) throw exception("%s: invalid AST",**name);

                //--------------------------------------------------------------
                //
                // get the name
                //
                //--------------------------------------------------------------
                const Tag parserName = GetName(ast);
                std::cerr << "parserName=" << parserName << std::endl;

                //--------------------------------------------------------------
                //
                // PreAnalyze
                //
                //--------------------------------------------------------------
                auto_ptr<Jive::Parser> parser = new Jive::Parser( parserName );
                {
                    PreAnalyzer A(name,parser.content());
                    A.walk( ast.content() );
                }
                return 0;
            }


            const string *Compiler:: GetName( XNode::Pointer &ast ) const
            {
                assert(ast->isInternal(Parser::DialectID));
                XList &children = ast->leaves();
                if(children.size<=0) throw exception("%s: empty %s", **name, Parser::DialectID);

                XNode *node = children.head;
                if( !node->isTerminal(Parser::GrammarID) ) throw exception("%s: missing %s for %s", **name, Parser::GrammarID, Parser::DialectID);
                const string id = node->lexeme()->toString(1);

                return Tags::Make(id);
            }


        }

    }

}
