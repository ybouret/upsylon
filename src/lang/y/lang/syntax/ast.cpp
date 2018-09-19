
#include "y/lang/syntax/terminal.hpp"
#include "y/lang/syntax/compound.hpp"

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
                    //__________________________________________________________
                    //
                    // left untouched, should be standard
                    //__________________________________________________________
                    return node;
                }
                else
                {
                    //__________________________________________________________
                    //
                    // recursive
                    //__________________________________________________________
                    const bool    agg  = (Aggregate::UUID==node->rule.uuid);
                    Node::List    &sub = node->children;
                    {
                        Node::List tmp;
                        while(sub.size)
                        {
                            //__________________________________________________
                            //
                            // apply AST to child
                            //__________________________________________________
                            Node *ch = AST(sub.pop_front());
                            if(ch->terminal)
                            {
                                //______________________________________________
                                //
                                // process terminal according to its attribute
                                //______________________________________________
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
                            else if( agg && Aggregate::UUID==ch->rule.uuid )
                            {
                                //______________________________________________
                                //
                                // check if merging content for acting
                                //______________________________________________
                                assert(ch->rule.data);
                                if( static_cast<const Compound *>(ch->rule.data)->type==Compound::Acting )
                                {
                                    tmp.merge_back(ch->children);
                                    delete ch;
                                }
                                else
                                {
                                    tmp << ch;
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

        }

    }
}

