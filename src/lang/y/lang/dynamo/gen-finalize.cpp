
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
            if(verbose) {
                indent() << std::endl;
                indent() << "---> Finalizing Top-Level Internals" << std::endl;
            }

            ++level;
            for( Internals::iterator i=internals.begin(); i != internals.end(); ++i)
            {
                Syntax::Compound &r = (**i).rule;
                if(r.type!=Syntax::Compound::Normal) continue; // already altered
                check_top_level(r);
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
            if(verbose) {
                indent() << "..checking internal <" << r.name << ">" << std::endl;
                indent() << "  \\__@" << r.typeName() << std::endl;
            }

            const Rule *producing = 0;

            for( Syntax::Operand *sub=r.head;sub;sub=sub->next)
            {
                if(verbose)
                {
                    indent() << "   |-" << sub->rule.typeName() << " / <" << sub->rule.name << ">"<< std::endl;
                }
                const Rule &rule = sub->rule;
                bool        look = false;
                switch(rule.uuid)
                {
                    case Syntax::Terminal::UUID:
                    {
                        assert(rule.data);
                        const Syntax::Terminal *t = static_cast<const Syntax::Terminal *>(rule.data);
                        if(t->attr!=Syntax::Terminal::Semantic)
                        {
                            look = true;
                        }
                    } break;

                    case Syntax::Aggregate::UUID:
                    case Syntax::Alternate::UUID:
                        look = true;
                        break;

                    default:
                        ;
                }
                if(!look) continue;

                if(producing)
                {
                    if(verbose) { indent() << "   \\_done" << std::endl; }
                    return;
                }

                producing=&rule;
                if(verbose) { indent() << "     \\_producing..." << std::endl; }
            }

            // at that point, 0 (a.k.a empty_something) or 1 or more items
            if(producing)
            {
                if(verbose)
                {
                    indent() << "   |-setting as a design compound" << std::endl;
                }
                //r.setDesign();
            }


        }


    }

}
