
#include "y/jive/dialect/parser.hpp"

namespace upsylon
{

    namespace Jive
    {

        namespace Dialect
        {

#define ID_HEAD "[_[:alnum:]]"

            Parser:: Parser() : Language::Parser("Dialect")
            {
                //--------------------------------------------------------------
                //
                // top level
                //
                //--------------------------------------------------------------
                Aggregate   &self = agg("dialect");
                const Axiom &END  = division(';');
                self << (var("module") << terminal("module_name", "\\." ID_HEAD ) << END);
                



                //--------------------------------------------------------------
                //
                // lexical rules
                //
                //--------------------------------------------------------------
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
