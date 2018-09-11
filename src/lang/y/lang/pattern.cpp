
#include "y/lang/pattern.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/graphviz.hpp"
#include "y/ios/osstream.hpp"
#include "y/codec/base64.hpp"

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

        void  Pattern:: link( const Pattern *p, ios::ostream  &os ) const
        {
            this->tag(os); os << "->"; p->tag(os); os << ";\n";
        }

        Pattern:: Pattern(const uint32_t t) throw() : uuid(t), next(0), prev(0), priv(0) {}


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

        string Pattern:: to_binary() const
        {
            string           ans;
            ios::osstream fp(ans);
            write(fp);
            return ans;
        }

        string Pattern:: to_base64() const
        {
            string           ans;
            {
                ios::osstream fp(ans);
                write(fp);
            }
            ios::base64::encoder b64;
            return b64.to_string(ans);
        }

        void Pattern:: NoMultiple( List &ops ) throw()
        {
            List                          tmp;
            while( ops.size )
            {
                const Pattern *lhs      = ops.head;
                bool           multiple = false;
                for(const Pattern *rhs=tmp.head;rhs;rhs=rhs->next)
                {
                    if( AreEqual(*lhs,*rhs) )
                    {
                        multiple = true;
                        break;
                    }
                }
                if(multiple)
                {
                    delete ops.pop_front();
                }
                else
                {
                    tmp.push_back( ops.pop_front() );
                }
            }
            ops.swap_with(tmp);
        }

    }

}

