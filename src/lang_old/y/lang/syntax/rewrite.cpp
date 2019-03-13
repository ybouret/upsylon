
#include "y/lang/syntax/terminal.hpp"
#include "y/lang/syntax/compound.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            Node * Node::Rewrite( Node *parent, const string &g)
            {
                assert(parent);
                if(parent->terminal)
                {
                    //__________________________________________________________
                    //
                    // simple terminal, left untouched
                    //__________________________________________________________
                    return parent;
                }
                else
                {
                    auto_ptr<Node>  guard(parent);
                    Node::List     &source = parent->children;
                    Node::SaveList  target;

                    while(source.size)
                    {
                        auto_ptr<Node> child = Rewrite(source.pop_front(),g);
                        if(child->terminal)
                        {
                            const Rule     &rule = child->rule; assert(Terminal::UUID==rule.uuid); assert(NULL!=rule.data);
                            const Terminal &term = *static_cast<const Terminal *>(rule.data);
                            switch (term.attr)
                            {
                                case Terminal::Standard:
                                    if(term.isOperator) throw exception("{%s} standard terminal %s shouldn't be an operator (will loose content)", *g, *(term.name));
                                    target.push_back( child.yield() );
                                    break;

                                case Terminal::Semantic:
                                    throw exception("{%s} unexpected semantic terminal %s and Node::Rewrite!!",*g,*(term.name));
                                    break;

                                case Terminal::Univocal:
                                    if(term.isOperator)
                                    {
                                        //--------------------------------------
                                        // this is where the rewriting
                                        // takes place: create an internal node
                                        // with the terminal rule...
                                        //--------------------------------------
                                        auto_ptr<Node> op = Node::Create(term);

                                        //--------------------------------------
                                        // take the LHS argument
                                        //--------------------------------------
                                        if(target.tail)
                                        {
                                            op->children.push_front( target.pop_back() );
                                        }

                                        //--------------------------------------
                                        // take the RHS argument
                                        //--------------------------------------
                                        if(source.head)
                                        {
                                            op->children.push_back( Rewrite(source.pop_front(),g) );
                                        }

                                        //--------------------------------------
                                        // insert op
                                        //--------------------------------------
                                        target.push_back( op.yield() );

                                        //--------------------------------------
                                        // child will be deleted
                                        //--------------------------------------
                                    }
                                    else
                                    {
                                        target.push_back(child.yield());
                                    }
                                    break;
                            }
                        }
                        else
                        {
                            target.push_back(child.yield());
                        }
                    }

                    target.swap_with(source);
                    return guard.yield();
                }
            }

            
        }

    }

}

