
#include "y/jive/dialect/parser.hpp"
#include "y/jive/lexical/plugin/comments.hpp"

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

                //--------------------------------------------------------------
                //
                //  module name
                //
                //--------------------------------------------------------------
                self << (var("module") << terminal("module_name", "\\." ID_EXPR ) << END);
                

                



                //--------------------------------------------------------------
                //
                // lexical rules
                //
                //--------------------------------------------------------------
                call( plug<C_Comments>("Comments") );
                call( plug<CppComments>("Comment") );

                {
                    const char blank__[] = "[:blank:]";
                    drop(blank__,blank__);
                }
                {
                    const char endl__[] = "[:endl:]";
                    silent_endl(endl__,endl__);
                }

                graphViz("dialect.dot");
                validate();
            }

            Parser:: ~Parser() throw()
            {
            }
            

        }
    }
}
