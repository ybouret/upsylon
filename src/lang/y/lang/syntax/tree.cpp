
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
                    const bool     parent_agg  = (Aggregate::UUID == parent_uuid);// || Terminal::UUID == parent_uuid );
                    while(source.size)
                    {
                        Node *child = Tree(source.pop_front(),g);
                        if(child->terminal)
                        {
                            //__________________________________________________
                            //
                            // process a terminal
                            //__________________________________________________
                            assert(Terminal::UUID==child->rule.uuid);assert(child->rule.data!=NULL);

                            const Terminal &term = *static_cast<const Terminal *>(child->rule.data);
                            const bool      isOp = term.isOperator;
                            switch( term.attr )
                            {
                                case Terminal::Standard:
                                    target << child;
                                    if(isOp) throw exception("{%s} standard terminal %s shouldn't be an operator", *g, *term.name);
                                    break;

                                case Terminal::Semantic:
                                    delete child;
                                    if(isOp)throw exception("{%s} semantic terminal %s shouldn't be an operator", *g, *term.name);
                                    break;

                                case Terminal::Univocal:
                                    child->lexeme.clear(); // clear content at once!
                                    if(false&&isOp)
                                    {
                                        delete child;
                                        //_____________________________________
                                        // modify the tree structure, create
                                        // a funky internal node
                                        // with a terminal structure
                                        //-------------------------------------
                                        auto_ptr<Node> opTree = Node::Create(term);
                                        std::cerr << "New Op " << opTree->rule.name << std::endl;

                                        //--------------------------------------
                                        // LHS
                                        //--------------------------------------
                                        if(target.tail)
                                        {
                                            opTree->children.push_front(target.pop_back());
                                        } else std::cerr << "No LHS" << std::endl;

                                        //--------------------------------------
                                        // RHS
                                        //--------------------------------------
                                        if(source.head)
                                        {
                                            opTree->children.push_back( Tree(source.pop_front(),g) );
                                        } else std::cerr << "No RHS" << std::endl;

                                        //--------------------------------------
                                        // grow
                                        //--------------------------------------
                                        target.push_back(opTree.yield());

                                    }
                                    else
                                    {
                                        //--------------------------------------
                                        // just append
                                        //--------------------------------------
                                        target << child;
                                    }
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


