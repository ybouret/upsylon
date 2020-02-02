
#include "y/lang/syntax/node/extended.hpp"
#include "y/lang/syntax/rule.hpp"
#include "y/string/convert.hpp"

namespace upsylon {

    namespace Lang {

        namespace Syntax {

            ExtendedNode:: ~ExtendedNode() throw() {}

            ExtendedNode:: ExtendedNode( const Rule &r, const string &s ) :
            InternalNode(r),
            shared( new string(s) )
            {
            }

            ExtendedNode:: ExtendedNode( const ExtendedNode &node ) throw() :
            object(),
            InternalNode(node),
            shared(node.shared)
            {
            }

            const string * ExtendedNode:: data() const throw()
            {
                return & *shared;
            }

            Node * ExtendedNode:: clone() const
            {
                return new ExtendedNode( *this );
            }


            size_t ExtendedNode:: serialize( ios::ostream &fp ) const
            {
                size_t ans = rule.name.serialize(fp);
                Y_OSTREAM_ADD_TO(ans,fp.emit_net,UUID);
                ans += shared->serialize(fp);
                return ans + serializeList(fp);
            }

            void  ExtendedNode::   vizCore( ios::ostream &fp ) const
            {
                const string l = string_convert::to_printable(rule.name);
                const string c = string_convert::to_printable(*shared);
                fp("[shape=house,label=\"%s='%s'\",style=rounded];\n",*l,*c);
                vizLink(fp);
            }

            const char *ExtendedNode:: className() const throw()
            {
                return "ExtendedNode";
            }


            Node * Node:: Create(const Rule &r, const string &s)
            {
                return new ExtendedNode(r,s);
            }
        }

    }

}
