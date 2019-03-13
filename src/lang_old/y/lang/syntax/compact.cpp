
#include "y/lang/syntax/terminal.hpp"
#include "y/lang/syntax/compound.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            static inline bool terminal_node_is_operator(const Node *node) throw()
            {
                assert(node);
                assert(node->terminal);
                if(node->rule.uuid==Terminal::UUID)
                {
                    assert(node->rule.data);
                    return static_cast<const Terminal *>(node->rule.data)->isOperator;
                }
                else
                {
                    return false;
                }
            }

            Node * Node:: Compact( Node *parent, bool &rewrite) throw()
            {
                assert(parent);
                if(parent->terminal)
                {
                    //__________________________________________________________
                    //
                    //
                    // left untouched
                    //
                    //__________________________________________________________
                    if( terminal_node_is_operator(parent) )
                    {
                        rewrite=true;
                    }
                    return parent;
                }
                else
                {
                    //__________________________________________________________
                    //
                    //
                    // study
                    //
                    //__________________________________________________________
                    const uint32_t parent_uuid = parent->rule.uuid;
                    const bool     would_merge = (Aggregate::UUID == parent_uuid) || (Terminal::UUID == parent_uuid);
                    Node::List    &source = parent->children;
                    Node::List     target; //!< no-throw, no need of C++ list

                    while(source.size>0)
                    {
                        Node       *child       = Compact(source.pop_front(),rewrite);
                        const Rule &child_rule  = child->rule;
                        const void *child_data  = child_rule.data; assert(child_data);
                        if(child->terminal)
                        {
                            //__________________________________________________
                            //
                            // process terminal child
                            //__________________________________________________
                            assert(Terminal::UUID==child_rule.uuid);
                            switch( static_cast<const Terminal *>(child_data)->attr )
                            {
                                case Terminal::Univocal: child->lexeme.clear(); // FALLTHRU
                                case Terminal::Standard: (target<<child); break;
                                case Terminal::Semantic: delete child;    break;
                            }
                        }
                        else
                        {
                            //__________________________________________________
                            //
                            // process internal child
                            //__________________________________________________
                            if(would_merge&&Aggregate::UUID==child_rule.uuid)
                            {
                                switch( static_cast<const Compound *>(child_data)->type )
                                {
                                    case Compound::Normal: (target<<child); break;
                                    case Compound::Acting: target.merge_back(child->children); delete child; break;
                                    case Compound::Design:
                                        if(child->children.size<=1)
                                        {
                                            target.merge_back(child->children); delete child;
                                        }
                                        else
                                        {
                                            (target<<child);
                                        }
                                        break;
                                }
                            }
                            else
                            {
                                (target<<child);
                            }
                        }
                    }
                    target.swap_with(source);
                    return parent;
                }

            }

            
        }

    }
}

