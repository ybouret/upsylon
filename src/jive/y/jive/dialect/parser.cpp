
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
                //--------------------------------------------------------------
                //
                // top level`
                //
                //--------------------------------------------------------------
                
                Aggregate &DIALECT = grp("Dialect");
                
                //--------------------------------------------------------------
                //
                // common terminals
                //
                //--------------------------------------------------------------
                const Axiom &STOP            = division(';');
                const Axiom &COLON           = division(':');
                const Axiom &BSTRING         = plugin<Lexical::bString>("bstring");
                const Axiom &JSTRING         = plugin<Lexical::jString>("jstring");
                const Axiom &RSTRING         = plugin<Lexical::rString>("rstring");
                const Axiom &STRING          = (alt("string") << JSTRING << RSTRING);
                
                //--------------------------------------------------------------
                //
                // grammar name declaration
                //
                //--------------------------------------------------------------
                {
                    Aggregate   &GRAMMAR = grp("Grammar_");
                    GRAMMAR << terminal("Grammar", "[.]" RULE_RX ) << STOP;
                    DIALECT <<  GRAMMAR;
                }
                
                //--------------------------------------------------------------
                //
                // Dialect Statements
                //
                //--------------------------------------------------------------
                Alternate &CODE = alt("Code");
                DIALECT << repeat(CODE,0);
                
                //--------------------------------------------------------------
                //
                // plugins
                //
                //--------------------------------------------------------------
                {
                    Aggregate &PLUGIN = agg("Plugin");
                    PLUGIN << terminal("PluginName", "@" RULE_RX) << COLON
                    << BSTRING
                    << ( repeat("PluginArgs",STRING,0) )
                    << STOP;
                    CODE << PLUGIN;
                }
                
                
                
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

