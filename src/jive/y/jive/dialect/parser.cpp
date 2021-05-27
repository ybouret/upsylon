
#include "y/jive/dialect/parser.hpp"
#include "y/jive/lexical/plugin/strings.hpp"
#include "y/jive/lexical/plugin/comments.hpp"

namespace upsylon
{
    
    namespace Jive
    {
        
        namespace Dialect
        {
            const char * const Parser::Include   = "Include";
            const char * const Parser::DialectID = "Dialect";
            const char * const Parser::GrammarID = "Grammar";
            const char * const Parser::bStringID = "bString";
            const char * const Parser::jStringID = "jString";
            const char * const Parser::rStringID = "rString";

            Parser:: ~Parser() throw()
            {
            }
            
#define HEAD_RX "[_[:alpha:]]"
#define TAIL_RX "[:word:]*"
#define NAME_RX  HEAD_RX TAIL_RX

            Parser:: Parser() : Jive::Parser("Dialect")
            {

                define("NAME",NAME_RX);



                //--------------------------------------------------------------
                //
                // top level
                //
                //--------------------------------------------------------------
                Aggregate   &DIALECT = agg(DialectID);
                const Axiom &END     = division(';');
                const Axiom &SEP     = division(':');
                const Axiom &BSTRING = plugin<Lexical::bString>(bStringID);
                const Axiom &JSTRING = plugin<Lexical::jString>(jStringID);
                const Axiom &RSTRING = plugin<Lexical::rString>(rStringID);
                const Axiom &STRING  = (alt("String") << JSTRING << RSTRING);
                
                //--------------------------------------------------------------
                //
                // Grammar Name
                //
                //--------------------------------------------------------------
                DIALECT << ( grp("GrammarDecl") <<  terminal("Grammar", "\\.{NAME}") << END );

                Alternate &STATEMENT = alt("Statement");
                DIALECT << repeat(STATEMENT,0);
                
                
                //--------------------------------------------------------------
                //
                // Plugin with args
                //
                //--------------------------------------------------------------
                {
                    Aggregate &PluginDecl = agg("Plugin");
                    PluginDecl << terminal("PluginName", "@{NAME}") << SEP << BSTRING;
                    PluginDecl << repeat("PluginArgs",STRING,0,false);
                    PluginDecl << END;
                    STATEMENT << PluginDecl;
                }
                
                //--------------------------------------------------------------
                //
                // include
                //
                //--------------------------------------------------------------
                {
                    Aggregate &IncludeDecl = agg(Include);
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
                validate(this);
                
            }
        }
    }
    
}

