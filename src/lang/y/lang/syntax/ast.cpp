
#include "y/lang/syntax/terminal.hpp"
#include "y/lang/syntax/compound.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

#if 0
            Node * Node::AST(Node *node, bool *needRewrite) throw()
            {
                assert(node);
                if(node->terminal)
                {
                    //__________________________________________________________
                    //
                    // left untouched, should be standard
                    //__________________________________________________________
                    if(needRewrite&&node->rule.uuid==Terminal::UUID&&static_cast<const Terminal *>(node->rule.data)->isOperator)
                    {
                        *needRewrite=true;
                    }
                    return node;
                }
                else
                {
                    //__________________________________________________________
                    //
                    // recursive
                    //__________________________________________________________
                    // possible if aggregate or modified terminal!!!
                    const bool     possible = (Aggregate::UUID==node->rule.uuid||Terminal::UUID==node->rule.uuid);
                    Node::List    &sub      = node->children;
                    {
                        Node::List tmp;
                        while(sub.size)
                        {
                            //__________________________________________________
                            //
                            // apply AST to child
                            //__________________________________________________
                            Node *ch = AST(sub.pop_front(),needRewrite);
                            if(ch->terminal)
                            {
                                //______________________________________________
                                //
                                // process terminal according to its attribute
                                //______________________________________________
                                const Rule      &rule = ch->rule; assert(rule.data);
                                const Terminal  &term = *static_cast<const Terminal *>(rule.data);
                                if(needRewrite&&term.isOperator)
                                {
                                    *needRewrite=true;
                                }
                                switch(term.attr)
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
                            else if( possible && Aggregate::UUID==ch->rule.uuid )
                            {
                                //______________________________________________
                                //
                                // check what to do with content
                                //______________________________________________
                                assert(ch->rule.data);
                                switch(static_cast<const Compound *>(ch->rule.data)->type)
                                {
                                    case Compound::Normal: tmp << ch; break;
                                    case Compound::Acting: tmp.merge_back(ch->children); delete ch; break;
                                    case Compound::Design:
                                        if(ch->children.size<=1)
                                        {
                                            tmp.merge_back(ch->children);
                                            delete ch;
                                        }
                                        else
                                        {
                                            tmp<<ch;
                                        }
                                        break;
                                }
                            }
                            else
                            {
                                //______________________________________________
                                //
                                // default: keep child as is
                                //______________________________________________
                                tmp << ch;
                            }
                        }
                        tmp.swap_with(sub);
                    }
                    return node;
                }
            }
#endif
            
        }

    }
}

