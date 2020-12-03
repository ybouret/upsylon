
#include "y/jive/dialect/compiler.hpp"

namespace upsylon
{

    namespace Jive
    {

        namespace Dialect
        {

            Compiler:: Compiler()
            {
            }

            Compiler:: ~Compiler() throw()
            {
            }


            static const char fn[] = "Dialect::Compiler: ";

            Jive::Parser * Compiler:: compileAST( XNode::Pointer &ast )
            {
                //--------------------------------------------------------------
                //
                // check
                //
                //--------------------------------------------------------------
                if( !ast->isInternal( Parser::DialectID ) ) throw exception("%s: invalid AST",fn);

                //--------------------------------------------------------------
                //
                // get the name
                //
                //--------------------------------------------------------------
                const Tag name = GetName(ast);
                std::cerr << "name=" << name << std::endl;
                return 0;
            }


            const string *Compiler:: GetName( XNode::Pointer &ast )
            {
                assert(ast->isInternal(Parser::DialectID));
                XList &children = ast->leaves();
                if(children.size<=0) throw exception("%s: empty %s", fn, Parser::DialectID);

                XNode *node = children.head;
                if( !node->isTerminal(Parser::GrammarID) ) throw exception("%s: missing %s for %s", fn, Parser::GrammarID, Parser::DialectID);
                const string id = node->lexeme()->toString(1);

                return Tags::Make(id);
            }


        }

    }

}
