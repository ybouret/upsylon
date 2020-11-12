
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
                Aggregate   &DIALECT = agg("Dialect");
                const Axiom &END     = division(';');
                const Axiom &SEP     = division(':');
                const Axiom &BSTRING = plugin<Lexical::bString>("bString");
                const Axiom &JSTRING = plugin<Lexical::jString>("jString");
                const Axiom &RSTRING = plugin<Lexical::rString>("rString");
                const Axiom &STRING  = (alt("String") << JSTRING << RSTRING);
                
                //--------------------------------------------------------------
                //
                // Grammar Name
                //
                //--------------------------------------------------------------
                DIALECT << ( grp("GrammarDecl") <<  terminal("Grammar", "\\." NAME_RX) << END );

                Alternate &STATEMENT = alt("Statement");
                DIALECT << repeat(STATEMENT,0);
                
                
                //--------------------------------------------------------------
                //
                // Plugin with args
                //
                //--------------------------------------------------------------
                {
                    Aggregate &PluginDecl = agg("Plugin");
                    PluginDecl << terminal("PluginName", "@" NAME_RX) << SEP << BSTRING;
                    PluginDecl << repeat("PluginArgs",STRING,0);
                    PluginDecl << END;
                    STATEMENT << PluginDecl;
                }
                
                //--------------------------------------------------------------
                //
                // include
                //
                //--------------------------------------------------------------
                {
                    Aggregate &IncludeDecl = agg("Include");
                    IncludeDecl << division("#include") << ( alt("IncludeName") << BSTRING << JSTRING );
                    STATEMENT << IncludeDecl;
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

