
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
#define NAME_RX  HEAD_RX TAIL_RX

            Parser:: Parser() : Jive::Parser("Dialect")
            {
                //--------------------------------------------------------------
                //
                // top level
                //
                //--------------------------------------------------------------
                Aggregate &DIALECT = agg("Dialect");

                const Axiom &END   = division(';');
                //const Axiom &NAME  = terminal("Name", NAME_RX);

                //--------------------------------------------------------------
                //
                // Grammar Name
                //
                //--------------------------------------------------------------
                DIALECT << ( grp() <<  terminal("Grammar", "\\." NAME_RX) << END );

                
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
                
                graphViz("dialect-grammar.dot");
                validate();
                
            }
        }
    }
    
}

