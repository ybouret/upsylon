
#include "y/lang/syntax/terminal.hpp"
#include "y/lang/syntax/compound.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            Node * Node:: Tree(Node *parent, const string &g)
            {
                assert(parent);
                if(parent->terminal)
                {
                    //----------------------------------------------------------
                    //
                    //
                    // simple terminal
                    //
                    //
                    //----------------------------------------------------------


                    return parent;
                }
                else
                {
                    //----------------------------------------------------------
                    //
                    //
                    // internal node => recursive and analyse
                    //
                    //
                    //----------------------------------------------------------
                    auto_ptr<Node> guard(parent);
                    ListType      &source = parent->children; // Raw List, deleted by ~Node()
                    SaveList       target;                    // C++ list, auto deleted
                    const uint32_t parent_uuid = parent->rule.uuid;
                    const bool     parent_agg  = (Aggregate::UUID == parent_uuid);
                    while(source.size)
                    {
                        Node *child = Tree(source.pop_front(),g);
                        if(child->terminal)
                        {
                            //__________________________________________________
                            //
                            // process a terminal
                            //__________________________________________________
                            assert(Terminal::UUID==child->rule.uuid);
                            assert(child->rule.data!=NULL);
                            switch( static_cast<const Terminal *>(child->rule.data)->attr )
                            {
                                case Terminal::Standard:
                                    target << child;
                                    break;

                                case Terminal::Semantic:
                                    delete child;
                                    break;

                                case Terminal::Univocal:
                                    child->lexeme.clear();
                                    target << child;
                                    break;
                            }


                        }
                        else
                        {
                            //__________________________________________________
                            //
                            // process another internal
                            //__________________________________________________
                            if( parent_agg && child->rule.uuid == Aggregate::UUID )
                            {
                                //----------------------------------------------
                                // possible fusion
                                //----------------------------------------------
                                assert(child->rule.data);
                                switch( static_cast<const Compound *>(child->rule.data)->type )
                                {
                                    case Compound::Normal: // add child to target
                                        target << child;
                                        break;
                                    case Compound::Acting: // add child content to target
                                        target.merge_back( child->children );
                                        delete child;
                                        break;

                                    case Compound::Design: // add child content only if size==1
                                        if( child->children.size <= 1)
                                        {
                                            target.merge_back(child->children);
                                            delete child;
                                        }
                                        else
                                        {
                                            target << child;
                                        }
                                        break;
                                }
                            }
                            else
                            {
                                //----------------------------------------------
                                // no possible fusion
                                //----------------------------------------------
                                target << child;
                            }

                        }
                    }

                    target.swap_with(source);
                    return guard.yield();
                }
            }
        }
    }

}


