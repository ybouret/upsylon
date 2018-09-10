
#include "y/lang/pattern.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/graphviz.hpp"

namespace upsylon
{
    namespace Lang
    {

        Pattern:: ~Pattern() throw()
        {
        }

        void Pattern::tag(ios::ostream &os) const
        {
            os << "p";
            os.viz(this);
        }

        Pattern:: Pattern(const uint32_t t) throw() : uuid(t) {}


        void Pattern:: viz( ios::ostream &os ) const
        {
            tag(os); __viz(os);
        }

        void Pattern:: GraphViz( const string &fn, bool keepFile) const
        {
            {
                ios::ocstream fp(fn);
                fp << "digraph G {\n";
                viz(fp);
                fp << "}\n";
            }
            ios::GraphViz::Render(fn,keepFile);
        }
    }

}

