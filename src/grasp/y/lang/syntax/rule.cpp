#include "y/lang/syntax/rule.hpp"
#include "y/string/convert.hpp"

namespace upsylon
{
    namespace  Lang
    {

        namespace Syntax
        {
            Rule:: ~Rule() throw()
            {
            }

            Rule:: Rule( const uint32_t i, const string &n ) :
            uuid(i),
            name(n),
            derived(0),
            verbose(false)
            {
            }

            void Rule:: graphVizName( ios::ostream &fp ) const
            {
                fp.viz(this);
            }

            void Rule:: graphVizLink( ios::ostream &fp, const Rule *p, unsigned idx ) const
            {
                assert(p);
                graphVizName(fp);
                fp << "->";
                p->graphVizName(fp);
                if(idx>0)
                {
                    fp("[label=\"%u\"]",idx);
                }
                fp << ";\n";
            }

            const char * Rule:: graphVizStyle() const throw()
            {
                if( isHollow() )
                {
                    return "dashed";
                }
                else
                {
                    return "solid";
                }
            }

            void Rule:: graphVizProlog( ios::ostream &fp ) const
            {
                graphVizName(fp);
                const string l = string_convert::to_printable(name);
                fp("[label=\"%s\",style=\"%s\",shape=\"%s\"];\n", *l, graphVizStyle(), graphVizShape() );
            }


        }
    }
}
