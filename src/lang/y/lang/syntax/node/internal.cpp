#include "y/lang/syntax/node/internal.hpp"
#include "y/lang/syntax/rule.hpp"
#include "y/string/convert.hpp"

namespace upsylon {

    namespace Lang {

        namespace Syntax {

            InternalNode:: ~InternalNode() throw()
            {
            }

            InternalNode:: InternalNode(const Rule &r) throw() :
            Node(r,false),
            Node::List()
            {
            }

            InternalNode:: InternalNode(const InternalNode &node) throw() :
            object(),
            Node(node),
            Node::List(node)
            {

            }


            Node * InternalNode:: clone() const
            {
                return new InternalNode(*this);
            }

            const void  * InternalNode:: inner() const throw()
            {
                return static_cast<const List *>(this);
            }

            void InternalNode::  vizLink( ios::ostream &fp ) const
            {
                const bool multiple = size>1;
                unsigned   idx      = 1;
                for(const Node *node = head; node; node=node->next, ++idx)
                {
                    node->vizSave(fp);
                    vizJoin(fp,node);
                    if(multiple) fp("[label=\"%u\"]",idx);
                    endl(fp);
                }
            }


            void     InternalNode::   vizCore( ios::ostream &fp ) const
            {
                const string l = string_convert::to_printable(rule.name);
                fp("[shape=house,label=\""); fp << l; fp("\"];\n");
                vizLink(fp);
            }




            size_t InternalNode:: serializeList(ios::ostream &fp) const
            {
                size_t ans = 0;
                Y_OSTREAM_ADD_TO(ans,fp.emit_upack,size);
                for(const Node *node=head;node;node=node->next)
                {
                    ans += node->serialize(fp);
                }
                return ans;
            }

            size_t InternalNode:: serialize(ios::ostream &fp) const
            {
                size_t ans = rule.name.serialize(fp);
                Y_OSTREAM_ADD_TO(ans,fp.emit_net,UUID);
                return ans + serializeList(fp);
            }

            const char *InternalNode:: className() const throw()
            {
                return "InternalNode";
            }

            const string * InternalNode:: data() const throw()
            {
                return 0;
            }


            Node * Node::Create(const Rule &r)
            {
                return new InternalNode(r);
            }

            void InternalNode:: returnTo(Lexer &lexer) throw()
            {
                while(size>0)
                {
                    tail->returnTo(lexer);
                    delete pop_back();
                }
            }

        }
    }
}

namespace upsylon {

    namespace Lang {

        namespace Syntax {

           
            
        }
    }
}
