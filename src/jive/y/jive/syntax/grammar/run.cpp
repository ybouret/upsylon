
#include "y/jive/syntax/grammar.hpp"
#include "y/exception.hpp"
#include "y/core/temporary-value.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {

#define Y_JIVE_GRAMLN(MSG) do { if(Axiom::Verbose) { std::cerr << "[[" << name << "]] " << MSG << std::endl; } } while(false)

            static inline
            void dispLexeme(const Tag    &name,
                            const char   *title,
                            const Lexeme *lx)
            {
                assert(title);
                if(lx)
                {
                    Y_JIVE_GRAMLN(title << "    = <" << lx->label << "> (= '" << *lx << "')" );
                }
                else
                {
                    Y_JIVE_GRAMLN(title << "    = (nil)");
                }
            }

            Node * Grammar:: run(Lexer &lexer, Source &source) const
            {
                //--------------------------------------------------------------
                // get root
                //--------------------------------------------------------------

                const Axiom *root = axioms.head;
                if(!root) throw exception("%s has no root Axiom!",**name);
                 Y_JIVE_GRAMLN("root=<"<<root->name<<">");

                //--------------------------------------------------------------
                // get status
                //--------------------------------------------------------------
                Node         *tree = NULL;
                Lexeme       *mind = NULL;
                const bool    success = root->accept(tree,lexer,source,mind,0);
                Node::Pointer xnode(tree) ;
                const Lexeme *next    = lexer.next(source);

                Y_JIVE_GRAMLN("success = " << success);
                Y_JIVE_GRAMLN("tree    = " << (tree!=NULL) );
                dispLexeme(name,"last",mind);
                dispLexeme(name,"next",next);

                //--------------------------------------------------------------
                // process status
                //--------------------------------------------------------------
                if(success)
                {
                    if(tree)
                    {
                        return Node::AST(xnode.yield());
                    }
                    else
                    {
                        return NULL;
                    }
                }
                else
                {
                    return NULL;
                }


             }

        }

    }

}

