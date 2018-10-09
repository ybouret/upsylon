#include "y/json/compiler.hpp"
#include "y/lang/dynamo.hpp"

namespace upsylon
{

    namespace
    {
#include "json.inc"
    }

    namespace JSON
    {
        class Compiler : public Lang::Dynamo
        {
        public:
            explicit Compiler() : Lang::Dynamo( "JSON_grammar", JSON_grammar, sizeof(JSON_grammar) )
            {
            }

            virtual ~Compiler() throw()
            {
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Compiler);
        };
    }

}
