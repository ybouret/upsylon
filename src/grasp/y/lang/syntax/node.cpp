#include "y/lang/syntax/node.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            Node:: ~Node() throw()
            {
            }

            Node:: Node(const Rule &r,
                        const bool term) throw() :
            Object(), Base(), rule(r), terminal(term), internal(!terminal)
            {
            }

            Node:: Node(const Node &other) throw() :
            Object(),
            Base(),
            rule(other.rule),
            terminal(other.terminal),
            internal(other.internal)
            {
            }


            Node * Node::Create(const Rule &r, Lexeme *l)
            {
                assert(l);
                try
                {
                    return new TerminalNode(r,l);
                }
                catch(...)
                {
                    delete l;
                    throw;
                }
            }

            Node * Node::Create(const Rule &r)
            {
                return new InternalNode(r);
            }

            const Lexeme & Node:: lexeme() const throw()
            {
                const void *data = inner();
                assert(terminal);
                assert(data);
                return *static_cast<const Lexeme *>(data);
            }

            Lexeme & Node:: lexeme() throw()
            {
                void *data = (void*)inner();
                assert(terminal);
                assert(data);
                return *static_cast<Lexeme *>(data);
            }

            const Node::List & Node:: children() const throw()
            {
                const void *data = inner();
                assert(internal);
                assert(data);
                return *static_cast<const List *>(data);
            }

            Node::List & Node:: children() throw()
            {
                void *data = (void*)inner();
                assert(internal);
                assert(data);
                return *static_cast<List *>(data);
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

        }

    }
}

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            TerminalNode:: ~TerminalNode() throw()
            {
            }

            TerminalNode:: TerminalNode(const Rule &r, Lexeme *l) throw() :
            Node(r,true),
            lx(l)
            {
                assert(NULL!=lx);
            }

            Node * TerminalNode:: clone() const
            {
                return Node::Create(rule, new Lexeme(*lx) );
            }

            const void  * TerminalNode:: inner() const throw()
            {
                assert(lx);
                return lx;
            }

        }
    }
}

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            InternalNode:: ~InternalNode() throw()
            {
            }

            InternalNode:: InternalNode(const Rule &r) throw() :
            Node(r,false), Node::List() //,children()
            {
            }

            InternalNode:: InternalNode(const InternalNode &node) :
            Node(node), //,children(node.children)
            Node::List(node)
            {

            }

            Node * InternalNode:: clone() const
            {
                return new InternalNode(*this);
            }

            const void  * InternalNode:: inner() const throw()
            {
                const Node::List &as_list = *this;
                return &as_list;
            }

        }
    }
}

