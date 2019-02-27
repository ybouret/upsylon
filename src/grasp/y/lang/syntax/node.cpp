#include "y/lang/syntax/node.hpp"

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
            Node(r,false),
            children()
            {
            }

            InternalNode:: InternalNode(const InternalNode &node) :
            Node(node),
            children(node.children)
            {

            }


            Node * InternalNode:: clone() const
            {
                return new InternalNode(*this);
            }

        }
    }
}

