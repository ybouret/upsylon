#include "y/lang/syntax/rule.hpp"
#include "y/exception.hpp"
#include "y/ios/tools/graphviz.hpp"
#include "y/string/convert.hpp"
#include "y/string/io.hpp"
#include "y/ios/osstream.hpp"
#include "y/ios/imstream.hpp"
#include "y/ios/null-ostream.hpp"

#include "y/codec/base64.hpp"

namespace upsylon {

    namespace Lang {

        namespace Syntax {

            Node:: ~Node() throw()
            {
            }

            Node:: Node(const Rule &r,
                        const bool term) throw() :
            Object(),
            Base(),
            Vizible(),
            Serializable(),
            rule(r),
            terminal(term),
            internal(!terminal)
            {
            }

            Node:: Node(const Node &other) throw() :
            object(),
            Object(),
            Base(),
            Vizible(),
            Serializable(),
            rule(other.rule),
            terminal(other.terminal),
            internal(other.internal)
            {
            }



            Node * Node::Create(const Rule &r)
            {
                return new InternalNode(r);
            }

            Node * Node:: Create(const Rule &r, const string &s)
            {
                return new ExtendedNode(r,s);
            }

            const Lexeme & Node:: lexeme() const throw()
            {
                const void *addr = inner();
                assert(terminal);
                assert(addr);
                return *static_cast<const Lexeme *>(addr);
            }

            Lexeme & Node:: lexeme() throw()
            {
                void *addr = (void*)inner();
                assert(terminal);
                assert(addr);
                return *static_cast<Lexeme *>(addr);
            }

            const Node::List & Node:: children() const throw()
            {
                const void *addr = inner();
                assert(internal);
                assert(addr);
                return *static_cast<const List *>(addr);
            }

            Node::List & Node:: children() throw()
            {
                void *addr = (void*)inner();
                assert(internal);
                assert(addr);
                return *static_cast<List *>(addr);
            }

            void  Node:: Grow( Node * &tree, Node *leaf ) throw()
            {
                assert(NULL!=leaf);
                if(NULL == tree )
                {
                    tree = leaf;
                }
                else
                {
                    assert(tree->internal);
                    tree->children().push_back(leaf);
                }
            }


            void InternalNode:: returnTo(Lexer &lexer) throw()
            {
                while(size>0)
                {
                    tail->returnTo(lexer);
                    delete pop_back();
                }
            }


            void   Node:: Unget( Node * &node, Lexer &lexer) throw()
            {
                assert(node);
                node->returnTo(lexer);
                delete node;
                node = 0;
            }



            void Node:: RemoveFrom( Node &node, Matching &name_matches )
            {
                if(node.internal)
                {
                    List &self = node.children();
                    List  temp;
                    while(self.size)
                    {
                        Node *sub = self.pop_front();
                        if(name_matches.exactly(sub->rule.name))
                        {
                            delete sub;
                        }
                        else
                        {
                            RemoveFrom(*sub,name_matches);
                            temp.push_back(sub);
                        }
                    }
                    temp.swap_with(self);
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


        }
    }
}

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

        }
    }
}



