
#include "y/lang/syntax/terminal.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {


            Node * Node::AST(Node *node) throw()
            {
                assert(node);
                if(node->terminal)
                {
                    // left untouched, should be standard
                    return node;
                }
                else
                {
                    Node::List &sub = node->children;
                    {
                        Node::List tmp;
                        while(sub.size)
                        {
                            Node *ch = AST(sub.pop_front());
                            if(ch->terminal)
                            {
                                const Rule               &rule = ch->rule; assert(rule.data);
                                const Terminal::Attribute attr = *static_cast<const Terminal::Attribute *>(rule.data);
                                switch(attr)
                                {
                                    case Terminal::Standard:
                                        tmp << ch;
                                        break;

                                    case Terminal::Univocal:
                                        ch->lexeme.clear();
                                        tmp << ch;
                                        break;

                                    case Terminal::Semantic:
                                        delete ch;
                                        break;
                                }
                            }
                            else
                            {
                                tmp << ch;
                            }
                        }
                        tmp.swap_with(sub);
                    }
                    return node;
                }
            }

        }

    }
}

