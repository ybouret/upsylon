
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


            namespace {

                static inline
                void __ManageTerminal( Node *sub,  Node::List &temp ) throw()
                {
                    assert(sub);
                    assert(sub->terminal);
                    assert(sub->rule.derived);
                    if(static_cast<const Terminal *>(sub->rule.derived)->isSemantic())
                    {
                        delete sub;
                    }
                    else
                    {
                        temp.push_back(sub);
                    }
                }

                static inline
                void __ManageInternal( Node *sub, Node::List &temp ) throw()
                {
                    assert(sub);
                    assert(sub->internal);
                    assert(sub->rule.derived);
                    const Rule &rule = sub->rule;

                    if( Aggregate::UUID == rule.uuid )
                    {
                        const Compound &compound = *static_cast<const Compound *>(rule.derived);
                        if(compound.behavior==Merge)
                        {
                            temp.merge_back(sub->children());
                            delete sub;
                        }
                        else
                        {
                            temp.push_back(sub);
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
                        node->lexeme().clear();
                    }

                    return node;
                }
                else
                {
                    // first pass: recursive call
                    Node::List &self = node->children();
                    {
                        Node::List  temp;
                        while(self.size)
                        {
                            Node *sub = AST(self.pop_front());

                            if(sub->terminal)
                            {
                                __ManageTerminal(sub,temp);
                            }
                            else
                            {
                                __ManageInternal(sub,temp);

                            }
                        }
                        self.swap_with(temp);
                    }

                    // check 1 item merging
                    if(1==self.size &&
                       Aggregate::UUID==self.head->rule.uuid &&
                       Merge == static_cast<const Compound *>(self.head->rule.derived)->behavior)
                    {
                        Node *sub = self.pop_front(); assert(0==self.size);
                        delete node;
                        return sub;
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
