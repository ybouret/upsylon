#include "y/lang/syntax/rule.hpp"
#include "y/exception.hpp"
#include "y/ios/tools/graphviz.hpp"
#include "y/string/convert.hpp"
#include "y/string/io.hpp"
#include "y/ios/osstream.hpp"
#include "y/ios/imstream.hpp"
#include "y/ios/null-ostream.hpp"

//#include "y/codec/base64.hpp"

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





