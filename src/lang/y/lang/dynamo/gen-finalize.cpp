
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
                if(r.type!=Syntax::Compound::Normal) continue; // already altered
                check_top_level(r);
            }
            --level;
        }


        void DynamoGenerator:: check_top_level( Syntax::Compound &r ) throw()
        {
            assert(Syntax::Compound::Normal==r.type);
            if(verbose) {
                indent() << "..checking internal <" << r.name << ">" << std::endl;
                indent() << "  \\__@" << r.typeName() << std::endl;
            }
            for( Syntax::Operand *sub=r.head;sub;sub=sub->next)
            {
                if(verbose)
                {
                    indent() << "   |-" << sub->rule.typeName() << " / <" << sub->rule.name << ">"<< std::endl;
                }
            }
        }


    }

}
