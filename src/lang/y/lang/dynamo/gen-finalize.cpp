
#include "y/lang/dynamo/generator.hpp"
#include "y/lang/syntax/joker.hpp"

//#include "y/exception.hpp"

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
            if(verbose) {
                indent() << std::endl;
                indent() << "---> Finalizing Top-Level Internals" << std::endl;
            }

            ++level;
            for( Internals::iterator i=internals.begin(); i != internals.end(); ++i)
            {
                Syntax::Compound &r = (**i).rule;
                if(r.type!=Syntax::Compound::Normal) continue; // already altered
                //check_top_level(r);
            }
            --level;

            if(verbose) {
                indent() << "---> done" << std::endl;
                indent() << std::endl;
            }
        }


        void DynamoGenerator:: check_top_level( Syntax::Compound &r ) throw()
        {
            assert(Syntax::Compound::Normal==r.type);
            if(verbose)
            {
                indent() << "..checking internal <" << r.name << ">" << std::endl;
                indent() << "  \\__@" << r.typeName() << std::endl;
                indent() << "   \\__#=" << r.size << std::endl;
            }
            if(2!=r.size)
            {
                return;
            }

            if(r.tail->rule.uuid == Syntax::Repeating::UUID )
            {
                const Syntax::Repeating *pRep = dynamic_cast<const Syntax::Repeating *>( & (r.tail->rule) );
                if(!pRep)
                {
                    throw exception("unexpected dynamic cast failure for Syntax::Repeating");
                }
                if(0==pRep->nmin)
                {
                    if(verbose)
                    {
                        indent() << "    \\__set as design!!!" << std::endl;
                    }
                    //r.setDesign();
                }
            }

        }


    }

}
