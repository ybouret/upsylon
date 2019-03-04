
#include "y/lang/syntax/grammar.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            const Terminal & Node:: asTerminal() const throw()
            {
                assert(terminal);
                assert(rule.derived);
                return *static_cast<const Terminal *>(rule.derived);
            }

            namespace
            {
                static inline
                void AST_Terminal( Node *sub, Node::List &temp )
                {
                    if(sub->asTerminal().isSemantic())
                    {
                        std::cerr << "ast.remove semantic <" << sub->rule.name << ">" << std::endl;
                        delete sub;
                    }
                    else
                    {
                        temp.push_back(sub);
                    }
                }

                static inline
                void AST_Internal( Node *sub, Node::List &temp )
                {
                    std::cerr << "ast.internal <" << sub->rule.name << ">, #=" << sub->children().size << std::endl;
                    if(sub->rule.uuid == Aggregate::UUID )
                    {
                        std::cerr << "ast.checking agg <" << sub->rule.name << ">" << std::endl;
                        switch( static_cast<const Compound *>(sub->rule.derived)->behavior )
                        {
                            case SubGroup:
                                temp.push_back(sub);
                                break;

                            case MergeAll:
                                std::cerr << "ast.merge all" << std::endl;
                                temp.merge_back(sub->children());
                                delete sub;
                                break;

                            case MergeOne:
                                if(1==sub->children().size)
                                {
                                    std::cerr << "ast.merge one" << std::endl;
                                    temp.merge_back(sub->children());
                                    delete sub;
                                }
                                else
                                {
                                    temp.push_back(sub);
                                }
                                break;
                        }
                    }
                    else
                    {
                        temp.push_back(sub);
                    }
                }
            }

            Node * Node:: AST(Node *node) throw()
            {
                assert(node);
                if(node->terminal)
                {
                    if(node->asTerminal().isUnivocal())
                    {
                        // delete content
                        std::cerr << "ast.clear univocal <" << node->rule.name << ">" << std::endl;
                        node->lexeme().clear();
                    }
                    return node;
                }
                else
                {
                    std::cerr << "ast.ini <" << node->rule.name << ">, #=" << node->children().size << std::endl;
                    Node::List &self = node->children();
                    {
                        Node::List temp;
                        while(self.size)
                        {
                            Node *sub = Node::AST( self.pop_front() );
                            if(sub->terminal)
                            {
                                AST_Terminal(sub,temp);
                            }
                            else
                            {
                                AST_Internal(sub,temp);
                            }
                        }
                        temp.swap_with(self);
                    }

                    std::cerr << "ast.fin <" << node->rule.name << ">, #=" << node->children().size << std::endl;
                    if(node->rule.uuid == Aggregate::UUID )
                    {
                        if( SubGroup == static_cast<const Compound *>(node->rule.derived)->behavior )
                        {
                            return node;
                        }
                        else
                        {
                            if(1==node->children().size)
                            {
                                Node  *one = node->children().pop_front();
                                delete node;
                                return one;
                            }
                            else
                            {
                                return node;
                            }
                        }
                    }
                    else
                    {
                        return node;
                    }
                }

            }


        }
    }
}
