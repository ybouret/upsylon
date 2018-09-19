
#include "y/lang/compiler.hpp"

namespace upsylon
{
    namespace Lang
    {
        Compiler:: ~Compiler() throw()
        {
        }

        Compiler:: Compiler( Syntax::Parser *p ) throw() :
        Syntax::Analyzer(),
        parser(p),
        name(parser->name)
        {
        }

        void Compiler:: compile( Module *module )
        {
            Source source(module);
            parser->reset();
            auto_ptr<Syntax::Node> ast = parser->parse(source);
            walk( *ast );
        }


    }

}
