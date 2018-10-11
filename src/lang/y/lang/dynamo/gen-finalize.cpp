
#include "y/lang/dynamo/generator.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    namespace Lang
    {

        void DynamoGenerator:: finalize() throw()
        {
            //__________________________________________________________________
            //
            // searching for design rules
            //__________________________________________________________________
            if(verbose) { indent() << "Finalizing top-level internals" << std::endl; }
            ++level;
            for( Internals::iterator i=internals.begin(); i != internals.end(); ++i)
            {
                Syntax::Compound &r = (**i).rule;
                if(verbose) { indent() << "..<" << r.name << ">" << std::endl; }
                if(r.type!=Syntax::Compound::Normal) continue; // already altered
                if(verbose) { indent() << "  \\_checking..." << std::endl; }
            }
            --level;
        }

    }

}
