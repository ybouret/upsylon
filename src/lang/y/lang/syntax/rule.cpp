
#include "y/lang/syntax/rule.hpp"
#include "y/string/convert.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            Rule:: ~Rule() throw()
            {
            }

            Rule:: Rule(const uint32_t t, const string &id) :
            uuid(t),
            name(id),
            next(0),
            prev(0)
            {
            }

            
            const char * Rule:: __shape() const throw() { return "box"; }
            const char * Rule:: __style() const throw() { return "solid"; }

            void Rule:: viz_id( ios::ostream &fp ) const
            {
                fp.viz(this);
            }

            void Rule:: prolog( ios::ostream &fp ) const
            {
                fp.viz(this);
                const string label = string_convert::to_printable(name);
                fp(" [label=\"%s\",shape=%s,style=%s];\n", *label, __shape(), __style());
            }

            void Rule::epilog( ios::ostream & ) const
            {
                
            }

        }
    }
}

