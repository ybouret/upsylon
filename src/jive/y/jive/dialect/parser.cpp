
#include "y/jive/dialect/parser.hpp"
#include "y/jive/lexical/plugin/strings.hpp"
#include "y/jive/lexical/plugin/comments.hpp"

namespace upsylon
{

    namespace Jive
    {

        namespace Dialect
        {
            Parser:: ~Parser() throw()
            {
            }

#define HEAD_RX "[_[:alpha:]]"
#define TAIL_RX "[:word:]*"
#define RULE_RX HEAD_RX TAIL_RX

            Parser:: Parser() : Jive::Parser("Dialect")
            {

                Aggregate   &GrammarNameDecl = agg("GrammarNameDecl");
                const Axiom &Stop            = division(';');
                GrammarNameDecl << terminal("GrammarName", "[.]" RULE_RX );
                GrammarNameDecl << Stop;



                //--------------------------------------------------------------
                //
                // comments
                //
                //--------------------------------------------------------------
                call( plug<Lexical::SingleLineComments>("comment","//"));
                call( plug<Lexical::MultiLinesComments>("comments","/\\*","\\*/"));

                //--------------------------------------------------------------
                //
                // lexical only
                //
                //--------------------------------------------------------------
                endl("endl","[:endl:]");
                drop("blank","[:blank:]");

                validate();
                graphViz("dialect-grammar.dot");
                
            }
        }
    }

}

