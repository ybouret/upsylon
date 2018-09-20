
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
            prev(0),
            data(0),
            verbose(false)
            {
            }

            void Rule:: Grow( Node * &tree, Node *child ) throw()
            {
                if(tree)
                {
                    assert(tree->internal);
                    tree->add(child);
                }
                else
                {
                    tree = child;
                }
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

            std::ostream & Rule:: display(std::ostream &os, const int depth, const char pfx ) const
            {
                for(int i=0;i<2*depth;++i) os << ' ';
                os << pfx << '<' << name << '>';
                return os;
            }


        }
    }
}

