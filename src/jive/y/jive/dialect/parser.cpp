
#include "y/jive/dialect/parser.hpp"
#include "y/jive/lexical/plugin/comments.hpp"
#include "y/jive/lexical/plugin/bstring.hpp"
#include "y/jive/lexical/plugin/strings.hpp"

namespace upsylon
{

    namespace Jive
    {

        namespace Dialect
        {

#define ID_HEAD "[_[:alnum:]]"
#define ID_TAIL "[:word:]*"
#define ID_EXPR ID_HEAD ID_TAIL

            Parser:: Parser() : Language::Parser("Dialect")
            {
                //--------------------------------------------------------------
                //
                // top level
                //
                //--------------------------------------------------------------
                Aggregate   &self = agg("dialect");
                const Axiom &END  = division(';');
                const Axiom &COL  = division(':');
                const Axiom &ID   = terminal("ID", ID_EXPR);
                const Axiom &BSTR = plugin<Lexical::bString>("bString");
                const Axiom &JSTR = plugin<Lexical::jString>("jString");
                const Axiom &RSTR = plugin<Lexical::rString>("rString");
                const Axiom &ASTR = pick( Axioms(JSTR,RSTR) );

                //--------------------------------------------------------------
                //
                //  module name
                //
                //--------------------------------------------------------------
                self << (grp("module_") << terminal("module", "\\." ID_EXPR ) << END);
                

                
                //--------------------------------------------------------------
                //
                //  rule definition
                //
                //--------------------------------------------------------------
                Aggregate &RULE = agg("rule");
                {
                    RULE << ID << COL;

                    RULE << END;
                }

                //--------------------------------------------------------------
                //
                //  plugin definition
                //
                //--------------------------------------------------------------
                Aggregate &PLUGIN = agg("plugin");
                {
                    PLUGIN << terminal( "plug", "@" ID_EXPR) << COL << BSTR << zeroOrMore(ASTR) << END;
                }

                //--------------------------------------------------------------
                //
                //  include defintion
                //
                //--------------------------------------------------------------
                Aggregate &INCLUDE = agg("include");
                {
                    INCLUDE << division("#include") << pick( Axioms(JSTR,BSTR) );
                }

                //--------------------------------------------------------------
                //
                //  content of grammar
                //
                //--------------------------------------------------------------
                self << zeroOrMore( pick( Axioms(RULE,PLUGIN,INCLUDE) ) );

                //--------------------------------------------------------------
                //
                // lexical rules
                //
                //--------------------------------------------------------------
                call( plug<C_Comments>("Comments") );
                call( plug<CppComments>("Comment") );

                drop("[:blank:]");
                silent_endl("[:endl:]");


                graphViz("dialect.dot");
                validate();
            }

            Parser:: ~Parser() throw()
            {
            }
            

        }
    }
}
