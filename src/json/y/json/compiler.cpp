#include "y/json/compiler.hpp"
#include "y/lang/dynamox.hpp"

namespace upsylon
{

    namespace
    {
#include "json.inc"
    }

    namespace JSON
    {
        class Compiler : public Lang::DynamoX, public singleton<Compiler>
        {
        public:
            Value value;

            void  createFrom( Lang::Module *module )
            {
                // initialize
                // value = nill
                compile(module);
            }

            void onTerminal( const string &name, const string &data )
            {
                Analyzer::onTerminal(name,data);
                space(std::cerr) << "code = 0x" << std::hex << terminalHash(name) << std::dec << std::endl;
                //std::cerr << "Terminal..." << std::endl;
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Compiler);
            explicit Compiler() :
            Lang::DynamoX( Y_DYNAMOX(JSON) ),
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
