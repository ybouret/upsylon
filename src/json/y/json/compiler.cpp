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
        class Compiler : public Lang::Dynamo, public singleton<Compiler>
        {
        public:
            Value value;

            void  createFrom( Lang::Module *module )
            {
                // initialize
                // value = nill
                compile(module);

            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Compiler);
            explicit Compiler() : Lang::Dynamo( "JSON_grammar", JSON_grammar, sizeof(JSON_grammar) ),
            value()
            {
            }

            virtual ~Compiler() throw()
            {
            }

            friend class singleton<Compiler>;
            static const at_exit::longevity life_time = ServerLifeTime-8;
        };


        void Load:: From( Lang::Module *module, Value & )
        {
            Compiler &json = Compiler::instance();
            json.createFrom(module);
        }

    }

}
